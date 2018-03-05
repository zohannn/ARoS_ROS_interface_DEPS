#include "VisionBotDlg.h"
#include "DialogInfoARoS.h"

#include <sstream>
#include <iomanip>

#include "MutexUtils.hpp"
#include "Timers.h"
#include "vector_operations.hpp"

IMPLEMENT_DYNAMIC(CDialogInfoARoS, CDialog)

CDialogInfoARoS::CDialogInfoARoS(CWnd* pParent /*=nullptr*/, CData* pDataStore /*=nullptr*/ )
	: CDialogInfo( IDD, pParent, pDataStore )
	, vTimerOn( Timer::InfoDlg::ID::NUMBER_OF_TIMERS, false )
{
	Create( IDD_DIALOG_INFO_AROS, pParent );

	// Setup object names to be used in Task: Toy Robot Learning
	vsObjects_ToyRobotLearning.clear();

	const int nTaskObjects = LearnTaskObj::NUMBER_OBJECTS_LEARN_TASK;
	
	vsObjects_ToyRobotLearning.assign( nTaskObjects, "" );

	for( int i=0 ; i<nTaskObjects ; i++ )
	{
		switch( i )
		{
		case LearnTaskObj::BA:
			vsObjects_ToyRobotLearning.at( i ) = "Base: \t";
			break;

		case LearnTaskObj::LW:
			vsObjects_ToyRobotLearning.at( i ) = "Left Wheel: ";
			break;
		
		case LearnTaskObj::LN:
			vsObjects_ToyRobotLearning.at( i ) = "Left Nut: ";
			break;
		
		case LearnTaskObj::RW:
			vsObjects_ToyRobotLearning.at( i ) = "Right Wheel: ";
			break;
		
		case LearnTaskObj::RN:
			vsObjects_ToyRobotLearning.at( i ) = "Right Nut: ";
			break;
		
		case LearnTaskObj::C1:
			vsObjects_ToyRobotLearning.at( i ) = "Column 1: ";
			break;
		
		case LearnTaskObj::C2:
			vsObjects_ToyRobotLearning.at( i ) = "Column 2: ";
			break;
		
		case LearnTaskObj::C3:
			vsObjects_ToyRobotLearning.at( i ) = "Column 3: ";
			break;

		case LearnTaskObj::C4:
			vsObjects_ToyRobotLearning.at( i ) = "Column 4: ";
			break;

		case LearnTaskObj::TF:
			vsObjects_ToyRobotLearning.at( i ) = "Top Floor: ";
			break;
		
		default:
			break;
		}
	}
}

CDialogInfoARoS::~CDialogInfoARoS()
{
	vTimerOn.clear();
}

void CDialogInfoARoS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OBJECT_INFO, m_static_object_info);
	DDX_Control(pDX, IDC_CHECK_UPDATE_OBJ_INFO, m_check_object_info);
	DDX_Control(pDX, IDC_STATIC_ACTIONS, m_static_actions);
	DDX_Control(pDX, IDC_CHECK_ACTIONS, m_check_actions);
	DDX_Control(pDX, IDC_STATIC_CSG, m_static_csg);
	DDX_Control(pDX, IDC_CHECK_CSG, m_check_csg);
}

BEGIN_MESSAGE_MAP(CDialogInfoARoS, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CDialogInfoARoS::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogInfoARoS::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogInfoARoS message handlers

BOOL CDialogInfoARoS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	Start_Info_Timers();
	
	//--------------------------------------------------------------------------

	m_check_object_info.SetCheck( BST_CHECKED );
	
	//--------------------------------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogInfoARoS::AlignWindow()
{
	RECT rectMain;
	pParent->GetWindowRect( &rectMain );
	
	int
		widthMain = static_cast<int>( rectMain.right - rectMain.left ),
		heightMain = static_cast<int>( rectMain.bottom - rectMain.top );
	
	RECT rect;
	GetWindowRect( &rect );
	
	int 
		width = static_cast<int>( rect.right - rect.left ),
		height = static_cast<int>( rect.bottom - rect.top );

	SetWindowPos( nullptr, rectMain.left, rectMain.bottom, width, height, SWP_NOZORDER );
}

std::tstring CDialogInfoARoS::get_Quantity_as_tstring( int quant )
{
	std::tstring str_quant( _T( "" ) );

	switch( quant )
	{
	case Quantity::ABSENT:
		str_quant = _T( "Absent" );
		break;
	
	case Quantity::IDLE:
		str_quant = _T( "Idle" );
		break;

	case Quantity::LOW:
		str_quant = _T( "Low" );
		break;

	case Quantity::MEDIUM:
		str_quant = _T( "Medium" );
		break;

	case Quantity::HIGH:
		str_quant = _T( "High" );
		break;

	default:
		str_quant = _T( "INVALID CATEGORY!" );
		break;
	}

	return str_quant;
}

void CDialogInfoARoS::UpdateInfo_Objects( void )
{
	if( m_check_object_info.GetCheck() == BST_UNCHECKED ) return;
	
	switch( pData->get_Task() )
	{
	default:
	case Task::CONSTRUCTION_TOY_FULL_SVS:
	case Task::TOY_ROBOT_LEARNING:
		UpdateInfo_Objects_Task_Construction();
		break;

	case Task::FACE_TRACKING:
		UpdateInfo_Objects_Task_FaceTracking();
		break;

	case Task::PLAY_PIANO:
		UpdateInfo_Objects_Task_PlayPiano();
		break;

	case Task::HR_TRANSPORT:
		UpdateInfo_Objects_Task_HRTransport();
		break;

	case Task::DIFF_KINEMATICS:
		UpdateInfo_Objects_Task_DiffKinematics();
		break;

	case Task::DRINKING:
		UpdateInfo_Objects_Task_Drinking();
		break;
	case Task::COMPUTER_ASSEMBLY:
		UpdateInfo_Objects_Task_ComputerAssembly();
		break;
	}
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_Construction( void )
{
	std::tstring
		str_info( _T( "" ) ),
		strObjName( _T( "" ) );

	std::vector< std::vector< CObject_f > > vObjectsCopy( NUMBER_OF_OBJECT_TYPES, std::vector< CObject_f >() );

	std::vector< std::tstring > str_obj( NUMBER_OF_OBJECT_TYPES, _T( "" ) );

	std::tstringstream str_text;
	str_text.precision( 4 );

	size_t
		i = 0,
		j = 0,
		n = 0,
		size = 0;
		
	int iObject = 0;

	//--------------------------------------------------------------------------
	// Create a copy of the objects that are in pData
	pData->vObjects_Shared.lock();

	for( i=0 ; i<NUMBER_OF_OBJECT_TYPES ; i++ )
		vector_copy( pData->vObjects_Shared.Obj.at( i ), vObjectsCopy.at( i ) );
	
	pData->vObjects_Shared.unlock();
	//--------------------------------------------------------------------------

	cv::Point3f
		coord( 0.0f, 0.0f, 0.0f ),
		ori( 0.0f, 0.0f, 0.0f );

	size = pData->get_TaskNumObjects();
	for( j=0 ; j<size ; j++ )
	{
		iObject = pData->get_TaskObject( static_cast<int>( j ) );
		n = vObjectsCopy.at( iObject ).size();
		str_obj.at( iObject ) = _T( "" );
		
		if( n==0 )
		{
			str_obj.at( iObject ) = _T( "\t" );
			str_obj.at( iObject ) += CA2T( CObject_f::get_ObjectName( iObject ).c_str() );
			str_obj.at( iObject ) += _T( "...\n" );
		}

		int 
			id = 0,
			id_obj = 0;

		for( i=0 ; i<n ; i++ )
		{
			if( vObjectsCopy.at( iObject ).at( i ).bVisible )
			{
				vObjectsCopy.at( iObject ).at( i ).get_Coordinates_World( &coord );
				vObjectsCopy.at( iObject ).at( i ).get_Orientation( &ori );

				str_text.str( _T( "" ) );

				strObjName = CA2T( vObjectsCopy.at( iObject ).at( i ).get_Name().c_str() );

				id_obj = vObjectsCopy.at( iObject ).at( i ).nObjectID;

				id = ( id_obj != -1 ) ? id_obj : static_cast<int>(i) +1;

				str_text
					<< strObjName
					<< " - #" << id << " ( "
					<< "X = " << coord.x / 10.0f << " cm ; "
					<< "Y = " << coord.y / 10.0f << " cm ; "
					<< "Z = " << coord.z / 10.0f << " cm ;";

				switch( vObjectsCopy.at( iObject ).at( i ).get_Type() )
				{
				case OBJECT_BASE:
					str_text << " phi = " << ori.z << "º )";
					break;

				case OBJECT_COLUMN_1:
				case OBJECT_COLUMN_2:
				case OBJECT_COLUMN_3:
				case OBJECT_COLUMN_4:
					str_text << " theta = " << ori.x << "º ; psi = " << ori.y << "º ; phi = " << ori.z << "º )";
					break;

				default:
					str_text << " )";
					break;
				}
			}

			str_text << std::endl;

			str_obj.at( iObject ) += str_text.str().c_str();
		}
	
		str_obj.at( iObject ) += _T( "----------------------------------------------\n" );
	}
	
	str_info = _T( "" );

	size = pData->get_TaskNumObjects();
	for( i=0 ; i<size ; i++ )
	{
		iObject = pData->get_TaskObject( static_cast<int>( i ) );
		str_info += str_obj.at( iObject );
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	for( i=0 ; i<NUMBER_OF_OBJECT_TYPES ; i++ )
	{
		vObjectsCopy.at( i ).clear();
		str_obj[i].clear();
	}

	size = str_obj.size();
	for( i=0 ; i<size ; i++ )
		str_obj.at( i ).clear();

	str_obj.clear();

	str_info.clear();
	strObjName.clear();
	str_text.clear();
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_FaceTracking( void )
{
	std::vector< CObject_f > vFaces;
	// ----------------------------------------------------------------------
	// Create a copy of all the faces detected
	pData->vObjects_Shared.lock();

	vector_copy( pData->vObjects_Shared.Obj.at( OBJECT_FACE ), vFaces );

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision( 4 );

	float 
		fPan = 0.0f,
		fTilt = 0.0f;

	int 
		id = 0,
		id_obj = 0;

	std::tstring 
		str_info( _T( "" ) ),
		strObjName( _T( "" ) );

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	size_t size = vFaces.size();

	for( size_t i=0 ; i<size ; i++ )
	{
		vFaces.at( i ).get_Coordinates_World( &coord );
		vFaces.at( i ).get_PanTiltAngles( &fPan, &fTilt );

		strObjName = CA2T( vFaces.at( i ).get_Name().c_str() );

		id_obj = vFaces.at( i ).nObjectID;

		id = ( id_obj != -1 ) ? id_obj : static_cast<int>( i ) + 1;

		str_text.str( _T( "" ) );
		str_text
			<< strObjName
			<< " - #"
			<< id << " ("
			<< " X = " << coord.x/10.0f << " cm ;"
			<< " Y = " << coord.y/10.0f << " cm ;"
			<< " Z = " << coord.z/10.0f << " cm ;"
			<< " Pan = " << fPan << "º ;"
			<< " Tilt = " << fTilt << "º"
			<< " )" << std::endl;

		str_info += str_text.str().c_str();
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	vFaces.clear();
	str_text.clear();
	str_info.clear();
	strObjName.clear();
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_PlayPiano( void )
{
	const int 
		TASK_OBJ[] = { OBJECT_RED, OBJECT_GREEN, OBJECT_BLUE, OBJECT_CYAN, OBJECT_MAGENTA, OBJECT_YELLOW },
		NOBJECTS = sizeof( TASK_OBJ ) / sizeof( int );

	if( NOBJECTS < 1 ) return;

	std::vector< std::vector< CObject_f > > vObjects( NOBJECTS, std::vector< CObject_f >() );

	size_t
		i = 0,
		size = 0,
		iObject = 0;

	// ----------------------------------------------------------------------
	// Create a copy of all the faces detected
	pData->vObjects_Shared.lock();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		if( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ).size() > 0 )
			vector_copy( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ), vObjects.at( i ) );
	}

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision( 4 );

	std::tstring 
		str_info = _T( "" ),
		strObjName = _T( "" );
	
	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	int
		id = 0,
		id_obj = 0;

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		str_text.str( _T( "" ) );
	
		size = vObjects.at( i ).size();

		if( size<1 ) continue;

		for( iObject=0 ; iObject<size ; iObject++ )
		{
			vObjects.at( i ).at( iObject ).get_Coordinates_World( &coord );

			strObjName = CA2T( vObjects.at( i ).at( iObject ).get_Name().c_str() );

			id_obj = vObjects.at( i ).at( iObject ).nObjectID;

			id = ( id_obj != -1 ) ? id_obj : static_cast<int>( iObject ) + 1;

			str_text
				<< strObjName
				<< " - #"
				<< id << " ("
				<< " X = " << coord.x/10.0f << " cm ;"
				<< " Y = " << coord.y/10.0f << " cm ;"
				<< " Z = " << coord.z/10.0f << " cm ;"
				<< " )" << std::endl;
		}
	
		str_info += str_text.str().c_str();
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	str_text.clear();
	str_info.clear();
	strObjName.clear();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		vObjects.at( i ).clear();
	}
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_HRTransport( void )
{
	const int
		TASK_OBJ[] = { OBJECT_GRASP_POINT },
		NOBJECTS = sizeof( TASK_OBJ ) / sizeof( int );

	std::vector< std::vector< CObject_f > > vObjects( NOBJECTS, std::vector< CObject_f >() );

	int i = 0;

	size_t
		size = 0,
		iObject = 0;

	// ----------------------------------------------------------------------
	// Create a copy of all the faces detected
	pData->vObjects_Shared.lock();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		if( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ).size() > 0 )
			vector_copy( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ), vObjects.at( i ) );
	}

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision( 4 );

	std::tstring 
		str_info = _T( "" ),
		strObjName = _T( "" );

	cv::Point3f coord( 0.0f, 0.0f, 0.0f );

	int 
		id = 0,
		id_obj = 0;

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		str_text.str( _T( "" ) );
	
		size = vObjects.at( i ).size();

		if( size<1 ) continue;

		for( iObject=0 ; iObject<size ; iObject++ )
		{
			vObjects.at( i ).at( iObject ).get_Coordinates_World( &coord );
			
			strObjName = CA2T( vObjects.at( i ).at( iObject ).get_Name().c_str() );
			
			id_obj = vObjects.at( i ).at( iObject ).nObjectID;

			id = ( id_obj != -1 ) ? id_obj : static_cast<int>( iObject ) + 1;

			str_text
				<< strObjName
				<< " - #"
				<< id << " ("
				<< " X = " << coord.x/10.0f << " cm ;"
				<< " Y = " << coord.y/10.0f << " cm ;"
				<< " Z = " << coord.z/10.0f << " cm ;"
				<< " )" << std::endl;
		}
	
		str_info += str_text.str().c_str();
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	str_text.clear();
	str_info.clear();
	strObjName.clear();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		vObjects.at( i ).clear();
	}
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_DiffKinematics( void )
{
	const int
		TASK_OBJ[] = { OBJECT_RED, OBJECT_GREEN, OBJECT_MAGENTA_SMALL, OBJECT_MAGENTA_BIG, OBJECT_YELLOW, OBJECT_HANDLE },
		NOBJECTS = sizeof( TASK_OBJ ) / sizeof( int );

	std::vector< std::vector< CObject_f > > vObjects( NOBJECTS, std::vector< CObject_f >() );

	size_t
		i = 0,
		size = 0,
		iObject = 0;

	// ----------------------------------------------------------------------
	// Create a copy of all the objects detected
	pData->vObjects_Shared.lock();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		if( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ).size() > 0 )
			vector_copy( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ), vObjects.at( i ) );
	}

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision( 4 );

	std::tstring 
		str_info = _T( "" ),
		strObjName = _T( "" );

	cv::Point3f 
		coord( 0.0f, 0.0f, 0.0f ),
		ori( 0.0f, 0.0f, 0.0f );

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		str_text.str( _T( "" ) );
	
		size = vObjects.at( i ).size();

		if( size<1 ) continue;

		int 
			id = 0,
			id_obj = 0;

		for( iObject=0 ; iObject<size ; iObject++ )
		{
			vObjects.at( i ).at( iObject ).get_Coordinates_World( &coord );
			
			strObjName = CA2T( vObjects.at( i ).at( iObject ).get_Name().c_str() );

			id_obj = vObjects.at( i ).at( iObject ).nObjectID;

			id = ( id_obj != -1 ) ? id_obj : static_cast<int>( iObject ) + 1;

			str_text
				<< strObjName
				<< " - #"
				<< id << " ("
				<< " X = " << coord.x/10.0f << " cm ;"
				<< " Y = " << coord.y/10.0f << " cm ;"
				<< " Z = " << coord.z/10.0f << " cm";


			if( vObjects.at( i ).at( iObject ).get_Type() == OBJECT_HANDLE )
			{
				vObjects.at( i ).at( iObject ).get_Orientation( &ori );
				str_text << " ; phi = " << ori.z << "º";
			}
			
			str_text << " )" << std::endl;
		}
	
		str_info += str_text.str().c_str();

		str_info += _T( "------------------------------------------------------------\n" );
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	str_text.clear();
	str_info.clear();
	strObjName.clear();

	for( i=0 ; i<NOBJECTS ; i++ )
	{
		vObjects.at( i ).clear();
	}
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_Drinking( void )
{
	const int
		TASK_OBJ[] = { OBJECT_GLASS, OBJECT_GLASS_INVERTED, OBJECT_BOTTLE },
		NOBJECTS = sizeof( TASK_OBJ ) / sizeof( int );

	std::vector< std::vector< CObject_f > > vObjects( NOBJECTS, std::vector< CObject_f >() );

	size_t
		i = 0,
		size = 0,
		iObject = 0;

	// ----------------------------------------------------------------------
	// Create a copy of all the objects detected
	pData->vObjects_Shared.lock();

	for( i = 0; i<NOBJECTS; i++ )
	{
		if( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ).size() > 0 )
			vector_copy( pData->vObjects_Shared.Obj.at( TASK_OBJ[i] ), vObjects.at( i ) );
	}

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision( 4 );

	std::tstring
		str_info = _T( "" ),
		strObjName = _T( "" );

	cv::Point3f	coord( 0.0f, 0.0f, 0.0f );

	for( i = 0; i<NOBJECTS; i++ )
	{
		str_text.str( _T( "" ) );

		size = vObjects.at( i ).size();

		if( size<1 ) continue;

		int
			id = 0,
			id_obj = 0;

		for( iObject = 0; iObject<size; iObject++ )
		{
			vObjects.at( i ).at( iObject ).get_Coordinates_World( &coord );

			strObjName = CA2T( vObjects.at( i ).at( iObject ).get_Name().c_str() );

			id_obj = vObjects.at( i ).at( iObject ).nObjectID;

			id = ( id_obj != -1 ) ? id_obj : static_cast<int>(iObject) +1;

			str_text
				<< strObjName
				<< " - #"
				<< id << " ("
				<< " X = " << coord.x / 10.0f << " cm ;"
				<< " Y = " << coord.y / 10.0f << " cm ;"
				<< " Z = " << coord.z / 10.0f << " cm";

			str_text << " )" << std::endl;
		}

		str_info += str_text.str().c_str();

		str_info += _T( "------------------------------------------------------------\n" );
	}

	m_static_object_info.SetWindowText( str_info.c_str() );

	str_text.clear();
	str_info.clear();
	strObjName.clear();

	for( i = 0; i<NOBJECTS; i++ )
	{
		vObjects.at( i ).clear();
	}
}

void CDialogInfoARoS::UpdateInfo_Objects_Task_ComputerAssembly(void)
{
	const int
		TASK_OBJ[] = { OBJECT_SCREWBOX, OBJECT_POWERSUPPLY, OBJECT_CABLE, OBJECT_HAND, OBJECT_SECONDHAND },
		NOBJECTS = sizeof(TASK_OBJ) / sizeof(int);

	std::vector< std::vector< CObject_f > > vObjects(NOBJECTS, std::vector< CObject_f >());

	std::vector< unsigned int > vComputerAssemblyTaskStatus;

	size_t
		i = 0,
		size = 0,
		iObject = 0;

	// ----------------------------------------------------------------------
	// Create a copy of all the objects detected
	pData->vObjects_Shared.lock();

	for (i = 0; i<NOBJECTS; i++)
	{
		if (pData->vObjects_Shared.Obj.at(TASK_OBJ[i]).size() > 0)
			vector_copy(pData->vObjects_Shared.Obj.at(TASK_OBJ[i]), vObjects.at(i));
	}

	pData->vObjects_Shared.unlock();
	// ----------------------------------------------------------------------

	std::tstringstream str_text;
	str_text.precision(4);

	std::tstring
		str_info = _T(""),
		strObjName = _T("");

	cv::Point3f	coord(0.0f, 0.0f, 0.0f);

	for (i = 0; i<NOBJECTS; i++)
	{
		str_text.str(_T(""));

		size = vObjects.at(i).size();

		if (size<1) continue;

		int
			id = 0,
			id_obj = 0;

		for (iObject = 0; iObject<size; iObject++)
		{
			vObjects.at(i).at(iObject).get_Coordinates_World(&coord);

			strObjName = CA2T(vObjects.at(i).at(iObject).get_Name().c_str());

			id_obj = vObjects.at(i).at(iObject).nObjectID;

			id = (id_obj != -1) ? id_obj : static_cast<int>(iObject)+1;

			str_text
				<< strObjName
				<< " - #"
				<< id << " ("
				<< " X = " << coord.x / 10.0f << " cm ;"
				<< " Y = " << coord.y / 10.0f << " cm ;"
				<< " Z = " << coord.z / 10.0f << " cm";

			str_text << " )" << std::endl;
		}

		str_info += str_text.str().c_str();

		str_info += _T("------------------------------------------------------------\n");
	}

	m_static_object_info.SetWindowText(str_info.c_str());

	str_text.clear();
	str_info.clear();
	strObjName.clear();

	for (i = 0; i<NOBJECTS; i++)
	{
		vObjects.at(i).clear();
	}
}

void CDialogInfoARoS::UpdateInfo_Actions( void )
{
	if( m_check_actions.GetCheck() == BST_UNCHECKED ) return;

	switch( pData->get_Task() )
	{
	default:
	case Task::CONSTRUCTION_TOY_FULL_SVS:
		UpdateInfo_Actions_Construction();
		break;

	case Task::DRINKING:
		UpdateInfo_Actions_Drinking();
		break;

	case Task::COMPUTER_ASSEMBLY:
		UpdateInfo_Actions_ComputerAssembling();
		break;
	}
}

void CDialogInfoARoS::UpdateInfo_Actions_Construction( void )
{
	std::vector< CObject_f > vHand;

	pData->vObjects_Shared.lock();
	vector_copy( pData->vObjects_Shared.Obj.at( OBJECT_HAND ), vHand );
	pData->vObjects_Shared.unlock();

	size_t
		iHand = 0,
		iObject = 0,
		nObjects = 0,
		nHands = vHand.size();

	std::tstringstream strm_aux( _T( "" ) );

	std::tstring 
		strText_Actions = _T( "" ),
		strObjName = _T( "" );

	// ----------------------------------------------------------------------
	// Hand actions
	for( iHand = 0 ; iHand < nHands ; iHand++ )
	{
		if( iHand != 0 ) strText_Actions += _T( "\n\n-------------------\n\n" );

		strm_aux.str( _T( "" ) );
		strm_aux << "Hand #" << iHand+1 << " near:" << std::endl;
		strText_Actions += strm_aux.str();
		
		nObjects = vHand.at( iHand ).vNearObjects.size();

		for( iObject = 0 ; iObject < nObjects ; iObject++ )
		{
			if( iObject != 0 ) strText_Actions += _T( " | " );
			strObjName = CA2T( vHand.at( iHand ).vNearObjects.at( iObject ).get_Name().c_str() );
			strText_Actions += strObjName;
		}
	}
	
	//--------------------------------------------------------------------------
	// Movement related actions
	
	float 
		fMov_Body = pData->HumanInfo.BodyMovement.get(),
		fMov_Hand = pData->HumanInfo.HandMovement.get();

	strm_aux.str( _T( "" ) );
	
	strm_aux
		<< std::endl << std::endl
		<< _T( "-------------------" )
		<< std::endl << std::endl
		<< _T( "Body movement: " ) << fMov_Body
		<< std::endl
		<< _T( "Hand movement: " ) << fMov_Hand;
	
	strText_Actions += strm_aux.str();

	//--------------------------------------------------------------------------
	// Write on GUI
	m_static_actions.SetWindowText( strText_Actions.c_str() );

	//--------------------------------------------------------------------------
	strm_aux.clear();
	strText_Actions.clear();
	strObjName.clear();
}

void CDialogInfoARoS::UpdateInfo_Actions_ComputerAssembling(void)
{
	std::vector< unsigned int> vTaskStatus;

	pData->vComputerAssemblyTaskStatus.lock();
	vector_copy(pData->vComputerAssemblyTaskStatus.Obj, vTaskStatus);
	pData->vComputerAssemblyTaskStatus.unlock();

	std::tstringstream strm_aux(_T(""));

	bool bFlag = false;


	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::ScrewBox) == 1);
	strm_aux << "ScrewBox present: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::PowerSupply) == 1);
	strm_aux << "Power Supply present: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::Cable) == 1);
	strm_aux << "Cable present: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::ScrewBox_HandOverPosition) == 1);
	strm_aux << "Hand Over ScrewBox: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::PowerSupply_HandOverPosition) == 1);
	strm_aux << "Hand Over Power Supply: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::Cable_HandOverPosition) == 1);
	strm_aux << "Hand Over Cable: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::HumanHand_Lifted) == 1);
	strm_aux << "Human hand in hand over position: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::SecondHumanHand_Lifted) == 1);
	strm_aux << "Second Human hand in hand over position: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = (vTaskStatus.at(LearningComputerAssemblyTask::VisionInfo::HumanGrasp) == 1);
	strm_aux << "Human Grasp: " << (bFlag ? "yes" : "no") << std::endl;

	//--------------------------------------------------------------------------
	// Write on GUI
	m_static_actions.SetWindowText(strm_aux.str().c_str());
}

void CDialogInfoARoS::UpdateInfo_Actions_Drinking( void )
{
	std::vector< unsigned int> vTaskStatus;

	pData->vDrinkTaskStatus.lock();
	vector_copy( pData->vDrinkTaskStatus.Obj, vTaskStatus );
	pData->vDrinkTaskStatus.unlock();

	std::tstringstream strm_aux( _T( "" ) );

	bool bFlag = false;

	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::Glass ) == 1 );
	strm_aux << "Glass present: " <<  ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::GlassInverted ) == 1 );
	strm_aux << "Glass Inverted: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::HandOverGlass ) == 1 );
	strm_aux << "Hand Over Glass: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::BottleInPlace ) == 1 );
	strm_aux << "Bottle In Place: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::BottleLifted ) == 1 );
	strm_aux << "Bottle Lifted: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::BottleTurned ) == 1 );
	strm_aux << "Bottle Turned: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::Hand_Reach_Glass ) == 1 );
	strm_aux << "Hand Reach Glass: " << ( bFlag ? "yes" : "no" ) << std::endl;
	
	bFlag = ( vTaskStatus.at(LearningDrinkingTask::VisionInfo::Hand_Release_Glass ) == 1) ;
	strm_aux << "Hand Release Glass: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = ( vTaskStatus.at( LearningDrinkingTask::VisionInfo::Hand_Reach_Inverted_Glass ) == 1 );
	strm_aux << "Hand Reach Inverted Glass: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at(LearningDrinkingTask::VisionInfo::Hand_Release_Inverted_Glass ) == 1 );
	strm_aux << "Hand Release Inverted Glass: " << (bFlag ? "yes" : "no") << std::endl;

	bFlag = ( vTaskStatus.at(LearningDrinkingTask::VisionInfo::Hand_Reach_Bottle) == 1 );
	strm_aux << "Hand Reach Bottle: " << ( bFlag ? "yes" : "no" ) << std::endl;
		
	bFlag = ( vTaskStatus.at(LearningDrinkingTask::VisionInfo::Hand_Release_Bottle ) == 1 );
	strm_aux << "Hand Release Bottle: " << (bFlag ? "yes" : "no") << std::endl;

	//--------------------------------------------------------------------------
	// Write on GUI
	m_static_actions.SetWindowText( strm_aux.str().c_str() );
}

void CDialogInfoARoS::UpdateInfo_CSG( void )
{
	if( m_check_csg.GetCheck() == BST_UNCHECKED ) return;

	int task = pData->get_Task();
	
	switch( task )
	{
	case Task::CONSTRUCTION_TOY_FULL_AVT:
	case Task::CONSTRUCTION_TOY_FULL_SVS:
	case Task::CONSTRUCTION_TOY_STEREO:
		UpdateInfo_CSG_ToyRobotConstruction();
		break;

	case Task::TOY_ROBOT_LEARNING:
		UpdateInfo_CSG_ToyRobotLearning();
		break;

	default:
		break;
	}
}

void CDialogInfoARoS::UpdateInfo_CSG_ToyRobotConstruction( void )
{
	if( m_check_csg.GetCheck() == BST_UNCHECKED ) return;

	std::tstring str_objects = _T( "" );

	size_t
		i = 0,
		j = 0,
		size = 0;

	//--------------------------------------------------------------------------
	// Copy data of inserted objects into local variables

	pData->vInsertedObjects_Shared.lock();

	std::vector< std::vector<int> > v_ins_obj( 2, std::vector<int>() );

	vector_copy( pData->vInsertedObjects_Shared.Obj.at( Side::HUMAN-1 ), v_ins_obj.at( Side::HUMAN-1 ) );
	vector_copy( pData->vInsertedObjects_Shared.Obj.at( Side::ROBOT-1 ), v_ins_obj.at( Side::ROBOT-1 ) );

	pData->vInsertedObjects_Shared.unlock();

	//--------------------------------------------------------------------------
	// Configure string with objects inserted on the base in the human and robot side
	str_objects += _T( "\nInserted objects on Human Side:\n" );
	size_t obj_size = v_ins_obj.at( Side::HUMAN-1 ).size();
	for( i=0 ; i < obj_size ; i++ )
	{
		str_objects += CA2T( CObject_f::get_ObjectName( v_ins_obj.at( Side::HUMAN-1 ).at( i ) ).c_str() );
		str_objects += _T( "\n" );
	}
	
	str_objects += _T( "\nInserted objects on Robot Side:\n" );
	obj_size = v_ins_obj.at( Side::ROBOT-1 ).size();
	for( i=0 ; i < obj_size ; i++ )
	{
		str_objects += CA2T( CObject_f::get_ObjectName( v_ins_obj.at( Side::ROBOT-1 ).at( i ) ).c_str() );
		str_objects += _T( "\n" );
	}

	//--------------------------------------------------------------------------
	// Configure string with information on column insertion
	std::tstringstream str_text;
	str_text.precision( 4 );

	cv::Point3f pt( 0.0f, 0.0f, 0.0f );

	CObject_f objBase_aux( pData->objBase );
	
	std::tstring strObjName = _T( "" );

	str_text << 
		std::endl << 
		_T( "----------------------------------------" ) << 
		std::endl;

	// Position of holes
	for( i = 0; i < objBase_aux.vSubObjects.size(); i++ )
	{
		objBase_aux.vSubObjects.at( i ).get_Coordinates_World( &pt );

		strObjName = CA2T( objBase_aux.vSubObjects.at( i ).get_Name().c_str() );
		
		str_text <<
			strObjName <<
			_T( " ( X: " ) << pt.x/10.0f <<
			_T( " cm ; Y: " ) << pt.y/10.0f <<
			_T( " cm ; Z: " ) << pt.z/10.0f <<
			_T( " cm )" ) << std::endl;
	}

	// Insertion status

	std::vector< std::pair < unsigned int, unsigned int > > vCol_Ins;

	mutex_vector_copy( pData->Column_InsertedAt_Shared, vCol_Ins );

	for( i = 0; i < vCol_Ins.size(); i++ )
	{
		strObjName = CA2T( CObject_f::get_ObjectName( vCol_Ins.at( i ).first ).c_str() );
		str_text << strObjName << _T( " -> " );
		strObjName = CA2T( CObject_f::get_ObjectName( vCol_Ins.at( i ).second ).c_str() );
		str_text << strObjName << std::endl;
	}

	//--------------------------------------------------------------------------
	// Print it!
	
	str_objects += str_text.str();
	str_objects += _T( "\0" );

	m_static_csg.SetWindowText( str_objects.c_str() );
	
	str_text.clear();
	str_objects.clear();
}

void CDialogInfoARoS::UpdateInfo_CSG_ToyRobotLearning( void )
{
	std::vector< unsigned int > vTaskStatus;

	mutex_vector_copy( pData->vTaskStatus, vTaskStatus );

	std::tstring str_csg = _T( "" );
	size_t status_size = vTaskStatus.size();
	for( size_t i=0 ; i<status_size ; i++ )
	{
		str_csg += CA2T( vsObjects_ToyRobotLearning.at( i ).c_str() );
		str_csg += _T( "\t" );
		str_csg += ( vTaskStatus.at( i ) == Side::INSERTED ) ? _T( "Inserted" ) : _T( "Not Inserted" );
		str_csg += _T( "\n" );
	}

	m_static_csg.SetWindowText( str_csg.c_str() );

	str_csg.clear();
}

void CDialogInfoARoS::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch( nIDEvent )
	{
		case Timer::InfoDlg::ID::OBJECT_INFO:
			UpdateInfo_Objects();
			break;

		case Timer::InfoDlg::ID::ACTIONS:
			UpdateInfo_Actions();
			break;

		case Timer::InfoDlg::ID::COMMON_SUB_GOALS:
			UpdateInfo_CSG();
			break;
	};

	CDialog::OnTimer(nIDEvent);
}

void CDialogInfoARoS::Start_Info_Timers( void )
{
	if( !vTimerOn.at( Timer::InfoDlg::ID::OBJECT_INFO ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::OBJECT_INFO ) = 
			( SetTimer( Timer::InfoDlg::ID::OBJECT_INFO, Timer::InfoDlg::Timeout::OBJECT_INFO, NULL ) != 0 );
	}
	
	if( !vTimerOn.at( Timer::InfoDlg::ID::ACTIONS ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::ACTIONS ) = 
			( SetTimer( Timer::InfoDlg::ID::ACTIONS, Timer::InfoDlg::Timeout::ACTIONS, NULL ) != 0 );
	}

	if( !vTimerOn.at( Timer::InfoDlg::ID::COMMON_SUB_GOALS ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::COMMON_SUB_GOALS ) = 
			( SetTimer( Timer::InfoDlg::ID::COMMON_SUB_GOALS, Timer::InfoDlg::Timeout::COMMON_SUB_GOALS, NULL ) != 0 );
	}
}

void CDialogInfoARoS::Stop_Info_Timers( void )
{
	if( vTimerOn.at( Timer::InfoDlg::ID::OBJECT_INFO ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::OBJECT_INFO ) = 
			( KillTimer( Timer::InfoDlg::ID::OBJECT_INFO ) == FALSE );
	}

	if( vTimerOn.at( Timer::InfoDlg::ID::ACTIONS ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::ACTIONS ) = 
			( KillTimer( Timer::InfoDlg::ID::ACTIONS ) == FALSE );
	}

	if( vTimerOn.at( Timer::InfoDlg::ID::COMMON_SUB_GOALS ) )
	{
		vTimerOn.at( Timer::InfoDlg::ID::COMMON_SUB_GOALS ) 
			= ( KillTimer( Timer::InfoDlg::ID::COMMON_SUB_GOALS ) == FALSE );
	}
}

void CDialogInfoARoS::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnOK();
}

void CDialogInfoARoS::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDialogInfoARoS::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	Stop_Info_Timers();
}
