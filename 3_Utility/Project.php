<?php
/**----------------------------------------------------------------------------
 * \file Project.php
 * \brief Class definitions for BIFACES tools
 *
 * \copyright Copyright (c) 2015-2018 Infineon Technologies AG. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *	 this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *	 this list of conditions and the following disclaimer in the documentation.
 *	 and/or other materials provided with the distribution.
 * - Neither the name of the copyright holders nor the names of its
 *	 contributors may be used to endorse or promote products derived from this
 *	 software without specific prior written permission.
 *	 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * To improve the quality of the software, users are encouraged to share
 * modifications, enhancements or bug fixes with Infineon Technologies AG 
 * (support@infineon.com).
 *
 * ----------------------------------------------------------------------------
 */
 
namespace Kernel\Project;


require_once (__DIR__ . '/../Configuration/Configuration.php');
require_once (__DIR__ . '/../ErrorReporting/ErrorReporting.php');
require_once ('Folder.php');
require_once (__DIR__ . '/../../Utilities/FileHandling.php');

use Kernel\Configuration\Configuration;
use Kernel\ErrorReporting\ErrorReport;
use Kernel\Project\Folder;
use Kernel\ErrorReporting\ERR;
use Kernel\ErrorReporting\Error;
use Utilities\FileHandling\FileNamePattern;

/**
 * This is the class to realize a project environment. A BIFACES project consists
 * of project configurations, and source files. The source files could be internal
 * or referenced from external folders. Project configurations are consisting of
 * providing the information about the external tooling, included/excluded source
 * files, external referenced files e.t.c
 * @created 23-May-2018 6:06:16 PM
 */
class Project
{

	private $configuration;		/**< \brief Project configuration values */
	private $errorReport;	  /**< \brief Error reporting for the project */
	private $name_ToBeRemoved;	   /**< \brief Project Name */
	private $sourceFolders = Array();	  /**< \brief List of source folders which are to be used for building the project */

	/**
	 * Constructor of the project object. This function shall take the project
	 * information as string. The input string is pre defined protocol and processed
	 * to feed the properties of project instance
	 * 
	 * @param inputString	 The input string is pre defined protocol and processed to
	 * feed the properties of project instance
	 */
	public function __construct($inputString)
	{
		//Instantiate a configuration object based on the input parameter string.


		//print("Input String: " . $inputString . PHP_EOL); //TODO : instrumentation code
		$this->errorReport= new ErrorReport(true, true, true);
		$this->configuration= new Configuration($inputString, $this->errorReport);
		$this->scanFolders();
	}

	/**
	 * Filters the files and folders from the selection configurations
	 * 
	 * @param elementList	 list of sub folders and files with in a folder
	 * @param selections	Selection configuration objects
	 * @param filteredElements	  List of filtered sub folders and files
	 */
	private function filterElements($elementList, $selections)
	{
		/* Start the latest filtered folder list with complete list of folders passed */
		$resultList= Array();
		$filtered= false;
	
		foreach($selections as $selection)
		{
			$isAsignedToProject= false;
			$newList= $this->filterForSelectionConfiguration($elementList, $selection);
		
			/* Check if the current select level is configured with assign type is not 'PROJECT' */
			if($selection->type !== 'PROJECT')
			{
				/* Run through next sub selections */
				$newList= $this->filterElements($newList, $selection->subSelections);  // RECURSIVE CALL !!!
				if($newList !== false) $isAsignedToProject= true; //At least one of the sub selections is assigned to project
			}
			else
			{
				$isAsignedToProject= true;
			}
		
			if($isAsignedToProject)
			{
				/* get the results in to filtered folder list, Remove duplicates */
				$newList= array_diff($resultList, $newList);
				$resultList= array_merge($resultFolderList, $newList);
				$filtered= true;
			}
		}
		return $filtered ? $resultList: false;
	}

	/**
	 * Filter the files against given select configurations
	 * 
	 * @param elementList	 List of files and folders
	 * @param selection	   Selection configuration
	 * @param returnList	Return element list after filtering
	 */
	private function filterForSelectionConfiguration($elementList, $selection)
	{
		/* Merge used file patterns */
		$useFilePatterns= Array();
		foreach($selection->usePatterns as $usePattern)
		{
			$useFilePatterns= array_merge($useFilePatterns, $usePattern->patterns);
		}
	
		/* Merge discard file patterns */
		$discardFilePatterns= Array();
		foreach($selection->discardPatterns as $discardPatterns)
		{
			$discardFilePatterns= array_merge($discardFilePatterns, $discardPatterns->patterns);
		}
	
		/* Filter only if there are use file patterns configured */
		if(count($useFilePatterns))
		{
			/* Get all the use patterns in to array and filter the folders "IN" */
			$flePatterns= new FileNamePattern($useFilePatterns);
			$elementList= $flePatterns->filterFilePathPattern(FileNamePattern::FILTER_IN, $elementList);
		}
	
		/* Filter only if there are discard file patterns configured */
		if(count($discardFilePatterns))
		{
			/* Get all the discard patterns in to array and filter the folders "OUT" */
			$flePatterns= new FileNamePattern($discardFilePatterns);
			$elementList= $flePatterns->filterFilePathPattern(FileNamePattern::FILTER_OUT, $elementList);
		}
	
		/* Filter for the variants */
		$elementList= $this->filterVariants($elementList, $selection->variants);
	
		/* return list */
		return $elementList;
	}

	/**
	 * Filters valid files from allowed file extensions from project configurations
	 * 
	 * @param fileList	  List of files
	 * @param returnList	Return file list after filter
	 */
	private function filterValidFiles($fileList)
	{
		/* Prepare the patterns from list of allowed file extensions */
		$validFileExtensions= Array();
	
		/* upend the array with valid file extensions */
		$fileExtensions= $this->configuration->fileExtensions;
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['C']);
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['ASM']);
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['CHEADER']);
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['ASMHEADER']);
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['MAKE']);
		$validFileExtensions= array_merge($validFileExtensions, $fileExtensions['OBJECT']);
	
		/* prepare the file pattern for all the files together*/
		$pattern= "*/*.(?:" . implode('|', $validFileExtensions) .")";
		$filePattern= new FileNamePattern($pattern);
	
		/* Filter the files	 and return them*/
		$validFiles= $filePattern->filterFilePathPattern(FileNamePattern::FILTER_IN, $fileList);
	
		return $validFiles;
	}

	/**
	 * Get project configuration object
	 * 
	 * @param configuration	   project configuration object
	 */
	public function getConfiguration()
	{
		return $this->configuration;
	}

	/**
	 * Gets array error report object
	 * 
	 * @param errorReport	 Error report object
	 */
	public function getErrorReport()
	{
		return $this->errorReport;
	}

	/**
	 * Gets array of source folder objects, which are listed in the project object
	 * 
	 * @param sourceFolders	   project configuration object
	 */
	public function getSourceFolders()
	{
		return $this->sourceFolders;
	}

	/**
	 * Lists all the elements with in the folder tree, which passed.
	 * 
	 * @param folderPathNames	 Folder names as array with path information for which
	 * sub folder information to be extracted
	 */
	private function listElements($folderPathNames)
	{
		//The function makes searches folder tree (non recursively). For any valid
		//folder/File an object is instanciated.


		/* To begin with, take the passed parameters as elements */
		$newElements= $folderPathNames;
		$folderNames= Array(); // Final list is initialized to empty list in the begining
		$fileNames= Array(); // Final list is initialized to empty list in the begining

		$FiletoOpen="0_Src/AppSw/Config/Common/Project_definition.h";
		if (file_exists($FiletoOpen)) {
			$file = fopen($FiletoOpen, "r");
			echo "Project_definition.h exists! ";
			while (!feof($file)) {
				$value = fgets($file);
				if(strpos ($value, "PROJECT_TRIBOARD")){
					print "Your project is PROJECT_TRIBOARD" . PHP_EOL;
//					$Exclude_pattern1="Triboard_Tricore";
					$Exclude_pattern1="PAGASUS2_Tricore";
					$Exclude_pattern2="V31C_Tricore";
					$Exclude_pattern3="V3D_Tricore";
					$Exclude_pattern4="V3NA_Tricore";
					$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
				elseif(strpos ($value, "PROJECT_PAGASUS2")){
					print "Your project is PROJECT_PAGASUS2" . PHP_EOL;
					$Exclude_pattern1="Triboard_Tricore";
//					$Exclude_pattern1="PAGASUS2_Tricore";
					$Exclude_pattern2="V31C_Tricore";
					$Exclude_pattern3="V3D_Tricore";
					$Exclude_pattern4="V3NA_Tricore";
					$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
				elseif(strpos ($value, "PROJECT_V31C")){
					print "Your project is PROJECT_V31C" . PHP_EOL;
					$Exclude_pattern1="Triboard_Tricore";
					$Exclude_pattern2="PAGASUS2_Tricore";
//					$Exclude_pattern2="V31C_Tricore";
					$Exclude_pattern3="V3D_Tricore";
					$Exclude_pattern4="V3NA_Tricore";
					$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
				elseif(strpos ($value, "PROJECT_V3D")){
					print "Your project is PROJECT_V3D" . PHP_EOL;
					$Exclude_pattern1="Triboard_Tricore";
					$Exclude_pattern2="PAGASUS2_Tricore";
					$Exclude_pattern3="V31C_Tricore";
//					$Exclude_pattern3="V3D_Tricore";
					$Exclude_pattern4="V3NA_Tricore";
					$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
				elseif(strpos ($value, "PROJECT_V3NA")){
					print "Your project is PROJECT_V3NA" . PHP_EOL;
					$Exclude_pattern1="Triboard_Tricore";
					$Exclude_pattern2="PAGASUS2_Tricore";
					$Exclude_pattern3="V31C_Tricore";
					$Exclude_pattern4="V3D_Tricore";
//					$Exclude_pattern4="V3NA_Tricore";
					$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
				elseif(strpos ($value, "PROJECT_V3AA")){
					print "Your project is PROJECT_V3AA" . PHP_EOL;
					$Exclude_pattern1="Triboard_Tricore";
					$Exclude_pattern2="PAGASUS2_Tricore";
					$Exclude_pattern3="V31C_Tricore";
					$Exclude_pattern4="V3D_Tricore";
					$Exclude_pattern5="V3NA_Tricore";
					//$Exclude_pattern5="V3AA_Tricore";
					$Found = "Yes";
				}
			}
		} else {
			echo "The file does not exist ";
			$Found = "No";
		}
	
		do
		{
			/* get files and dirs separated */
			$newFolderNames= array_filter($newElements, 'is_dir');
			$newFileNames= array_filter($newElements, 'is_file');
	
			/* Filter the files, based on the selection configuration, from list */
			$filterResult= $this->configuration->selectForProject($newFileNames);
		
			/* filter result is false if files are not filtered, if not false, they contain only fildered results */
			$newFileNames= ($filterResult !== false) ? $filterResult : $newFileNames;
		
			/* Add filtered list to the final list */
			$folderNames= array_merge($folderNames, $newFolderNames);
			$fileNames= array_merge($fileNames, $newFileNames);
	
			$newElements=Array();	// Starts with next level a fresh
			foreach($newFolderNames as $folderName) 
			{
				/* Fetch all the sub folder elements for each folder at this level */
				$folderElements= glob($folderName.'/*', GLOB_NOSORT);

				if($Found == "Yes"){
					if(strpos ($folderName, $Exclude_pattern1) || 
						strpos ($folderName, $Exclude_pattern2) || 
						strpos ($folderName, $Exclude_pattern3) ||
						strpos ($folderName, $Exclude_pattern4) ||
                        strpos ($folderName, $Exclude_pattern5)){
						/* Get the root folder path replaced */
						print("Debug:" . $folderName . " excluded" . PHP_EOL);;
						continue;
					}
				}


				/* Check if there is make file already available, if yes list only hat file */
				/* TODO : IFU to user manual */
				/** Infrormation for usage: For any folder with in source folders, project may define its own make files.
				 * Such make files shall take care of processing the elements(sub folder tree and files) which are present
				 * in the folder. BIFACES will not scan such sub folder tree and files. It will include the available make
				 * files for make generation. With such custom made make files, user must supply make variables as
				 * specified by BIFACES.
				 */
				$makeFiles= $this->listMakeFiles($folderElements);

				
				if(count($makeFiles) > 0)
				{
					$newElements= array_merge($newElements, $makeFiles); // Yes, only make files form this level at this branch
				}
				else
				{
					$newElements= array_merge($newElements, $folderElements);
				}
			}
		}while(count($newElements) !== 0); // Iterate untill there are no more elements in foldres
	
		/* now instanciate the folder object for each of the usable folders*/
		foreach($folderNames as $folderName)
		{
			$this->sourceFolders[$folderName]= new Folder($folderName, $this->configuration);
		}

		/* Filter for valid files */
		$fileNames= $this->filterValidFiles($fileNames);
		
		/* now instanciate the file objects for each of the usable folders*/
		foreach($fileNames as $fileName)
		{
			$folderName= dirname($fileName);
			$folder= $this->sourceFolders[$folderName];
			$file= new File($fileName, $this->configuration);
			$folder->addFile($file);
		}
	
		/* Update folder internals with added file information */
		foreach($this->sourceFolders as $folder)
		{
			$folder->classifyFiles();
			$folder->updateSubFolderNames(array_keys($this->sourceFolders));
		}
	
		/* Update each file to select the assignees */
		$this->updatefileAssignments($fileNames);
	}

	/**
	 * Filters the make files if available
	 * 
	 * @param makeFileList	  List of make files
	 * @param elementList	 List of elements in the folder
	 */
	private function listMakeFiles($elementList)
	{
		$makeFileList= Array();
		$makeFileExtensions= $this->configuration->fileExtensions['MAKE'];
	
		/* prepare the file pattern for all the files together*/
		$pattern= "*/*.(?:" . implode('|', $makeFileExtensions) .")";
		$filePattern= new FileNamePattern($pattern);
	
		/* Filter the files	 and return them*/
		$makeFiles= $filePattern->filterFilePathPattern(FileNamePattern::FILTER_IN, $elementList);

		if(count($makeFiles) > 0)
		{
			/* Filter only files from the list */
			$makeFileList= array_filter($makeFiles, 'is_file');
		}
	
		return $makeFileList;
	}

	/**
	 * Scans the project source folders to list valid folders, which are to be used
	 * for building the target.
	 * Scanned folders include both internal and external folders.
	 */
	private function scanFolders()
	{
		//For each root source folders, instanciate the folder object. get folder tree
		//with all valid folders listed.
		//Instanciate folder object for each of those folders.

		/* Get the list of source folders and validate if they really exist */
		$sourceFolderNames= Array();
		foreach($this->configuration->sourceFolders as $sourceFolderName)
		{
			if(file_exists($sourceFolderName))
			{
				$sourceFolderNames[]= $sourceFolderName;
			}
			else
			{
				$errorText= "Configured source folder, '" . $sourceFolderName . "'	doesn't exist!";
				$error= new Error(ERR::TW, ERR::ID_W_CONFIG_MISSING_FOLDER, $errorText);
				$this->errorReport->register($error);
			}
		}
		/* list all the elements in folders tree */
		$this->listElements($sourceFolderNames);
	}

	/**
	 * Update the files for each type for assignee
	 * 
	 * @param inputList	   Input element(file) list to be used for assignment
	 */
	private function updateFileAssignments($inputList)
	{
		if(count($this->configuration->architectures) > 0)
		{
			$assignees= $this->configuration->assignFiles($inputList, $this->errorReport);
			if(isset($assignees['TARGET']))
			{
				$this->updateFileTargets($assignees['TARGET']);
			}
	
			/* Assign primary target to files without target */
			$this->updateFilePrimaryTargets();
	
			if(isset($assignees['TOOL']))
			{
				$this->updateFileTools($assignees['TOOL']);
			}
	
			if(isset($assignees['OPTIONSET']))
			{
				$this->updateFileOptionSets($assignees['OPTIONSET']);
			}
		}
	}

	/**
	 * Update the option set assigns to the files
	 * 
	 * @param optionAssigns	   Configuration for file assignment to option sets
	 */
	private function updateFileOptionSets($optionAssigns)
	{
		foreach($optionAssigns as $optionSetId => $filePathNames)
		{
			foreach($filePathNames as $filePathName)
			{
				$folder= $this->sourceFolders[dirname($filePathName)];
				$file= $folder->getFile($filePathName);
				$file->updateOptionSet($optionSetId);
			}
		}
	}

	/**
	 * Updates the files, which are not configured with any targets with primary
	 * targets
	 */
	private function updateFilePrimaryTargets()
	{
		$primaryArchitectureName= $this->configuration->getPrimaryArchitectureName();
		$primaryArchitecture= $this->configuration->architectures[$primaryArchitectureName];
		$primaryTargetName= $primaryArchitectureName . "/" . $primaryArchitecture->getPrimaryTargetName();
	
		foreach($this->sourceFolders as $folder)
		{
			foreach($folder->getFilePathNames() as $filePathName)
			{
				$file= $folder->getFile($filePathName);
				if(count($file->getTargetFullNames()) === 0)
				{
					$file->updateTarget($primaryTargetName);
				}
			}
		}
	}

	/**
	 * Update the target assigns to the files
	 * 
	 * @param targetAssigns	   Configuration for file assignment to targets
	 */
	private function updateFileTargets($targetAssigns)
	{
		foreach($targetAssigns as $targetName => $fileNames)
		{
			foreach($fileNames as $fileName)
			{
				$folder= $this->sourceFolders[dirname($fileName)];
				$file= $folder->getFile($fileName);
				$file->updateTarget($targetName);
			}
		}
	}

	/**
	 * Update the tool assigns to the files
	 * 
	 * @param toolAssigns	 Configuration for file assignment to tools
	 */
	private function updateFileTools($toolAssigns)
	{
		foreach($toolAssigns as $toolName => $filePathNames)
		{
			foreach($filePathNames as $filePathName)
			{
				$folder= $this->sourceFolders[dirname($filePathName)];
				$file= $folder->getFile($filePathName);
				$file->updateTool($toolName);
			}
		}
	}

}
?>
