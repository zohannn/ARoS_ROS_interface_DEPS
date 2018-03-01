#ifndef _FILES_DIRECTORIES_H_
#define _FILES_DIRECTORIES_H_

#pragma once

#include <string>
#include <vector>

///	<summary>Checks if the specified file exists.</summary>
///	<param name="file_name">File name to check.</param>
///	<returns>true if file exists, otherwise false.</returns>
bool FileExists( std::string file_name );

///	<summary>Searches for a file in some default directories (./ , ../, ../../) 
/// and also some specified extra directories with a few variations
/// (config, conf).</summary>
///	<param name="file_name">Name of the file to search.</param>
///	<param name="file_path">If the function is sucessfull, this parameter will hold the
/// full path of the file.</param>
///	<param name="vecExtraDirectories">Vector with extra directories to add to the search.</param>
///	<param name="vecDirVariations">Vector with extra variations to add to the search.</param>
///	<returns>true if the file is found, otherwise false.</returns>
bool Find_File( std::string file_name, std::string &file_path, std::vector< std::string > vecExtraDirectories = std::vector< std::string >(), std::vector< std::string > vecDirVariations = std::vector< std::string >() );

///	<summary>Checks if the specified direcory exists.</summary>
///	<param name="path_dir">Path to directory to test.</param>
///	<returns>true if directory exists, otherwise false.</returns>
bool DirectoryExists( std::string path_dir );

///	<summary>Creates a directory.</summary>
///	<param name="dir_path">path to directory to create.</param>
///	<returns>true if the directory was created, otherwise false.</returns>
bool CreateDirectory( std::string dir_path );

#endif //_FILES_DIRECTORIES_H_
