/*
Marcus Williams
Lab 4 File System
April 30 2017
*/

#include <stdio.h>
//Need to add all the basic libraries and then some system specific ones



/*File System Specs:
						Block Size: 512 bytes
						Max file size: 32786 characters
						Address Space: 2^32 bytes

*/
 

//Create new Disk and mount in OS. The 10 MB file "Disk" must be declared as a disk device
//	before we can use it as storage
//Here we are going to use new_disk, but maybe we'll have to use fsmk() 
int new_disk(disk_t *disk, char *name);

//Make Super block and write it to first block in new disk
int create_sb(){

		//Define structure of Super block
		struct my_super_block sb = 
		{
			.magic = SIMPLEFS_MAGIC,
			.block_size = 512,
			.inodes_count = INODE_NUMBER,
			.free_blocks = total_used - INODE_NUMBER,
		};


		//Write Super Block to disk

		number_of_bytes = fwrite(disk, &sb,512);

		if(number_of_bytes != to default block size there is an error)
		{
			printf("Error creating super Block\n");
			report error
			return -1;
		}

		printf("Super Block written Successfully\n");
		return 0;
}

//Make Root Node and write it to disk
int make_root(){

		//define inode structure here
		struct fs_inode 
		{
			mode_t mode;   //Use mode to define what type of node it is directory, file, root
			uint64_t inode_no; // ???
			uint64_t data_block_number; // ???
			int file_type; // Use this to store the type of file being created by
						   // 	extenstion type. Use 0 for directory?

		}

	//Create root inode for new disk... 
	struct fs_inode root_node

			root_node.mode = ...
			root_node.inode_no = ...
			root_node.data_block_number = ...
			root_node.file_type = 0; // for directory

	//Write root to disk as a starting point for all others
    number_of_bytes = fwrite(fd, ....);

    if(number_of_bytes != to default block size)
    	{
    		printf("Error creating root\n");
			report error
			return -1;
		}

		printf("Root written Successfully\n");
		return 0;


}

//This section will be in a .h file full of functions
int create_directory(char *new_dir_name,char* parent_directory)
{
	struct fs_inode new_node

	//Set new_node characteristics...

			new_node.mode = ...
			new_node.inode_no = ...
			new_node.data_block_number = ...
			new_node.file_type = 0; // for directory

	// Place new directory into hierarchy....
		/* 1)   if(no directory provided)
					Get current directory name
		        else
		    		Use provided directory	

		   2)	Add new directory as child of parent directory

		*/


}

int create_file(char *new_file, char *parent_directory)
{
	struct fs_inode new_node

	//Set new_node characteristics...

			new_node.mode = ... set this node as file
			new_node.inode_no = ...
			new_node.data_block_number = ...
			new_node.file_type = ...

	// Place new directory into hierarchy....
		/* 1)   if(no directory provided)
					Get current directory name
		        else
		    		Use provided directory	

		   2)	Add new file as child of parent directory

		*/

}

int delete_file(char *file_to_remove, char *parent_directory)
{
	int remove(char *file_to_remove);

	//Check if file_to_remove is a directory or file..
		/* 1)	if (we are removing a file)
					Then ask for conformation*/
					printf("Are you sure you want to delete file %s?(Yes/No):");
					remove(file_to_remove);

	  /*   2)	else(we are removing a directory)
	  				if(directory is empty)*/
	  					printf("Are you sure you want to delete Directory %s?(Yes/No):");
	  					remove(file_to_remove);
	  			/*	else(directory is not empty)
	  					print warning...*/
	  					printf("Warning!! Directory is not empty\nDeleting %s will also delete its contents\nAre you sure you want to continue?(Yes/No):");
	  			//		for(All file and directories in file_to_remove)
	  					{
							remove(each file in file_to_remove);
	  					}
	  						remove(file_to_remove);


	// Remove file from directory....
	int remove(char *file_to_remove)
	{
		/* 1)   if(no directory provided)
					Get current directory name
		        else
		    		Use provided directory	

		   2)	Remove child file from parent directory

		*/
	}

}

int open_file(char *file_to_open, char* parent_directory)
{
	//Check if file exist
		if(file exist)
			err = fopen(file_to_open)
			//check for error
			if(err != 0)
				printf("Failed to open file\n");
		else(file does not exist)
			//check for error
			err = create_file(file_to_open, parent_directory)
			if(err != 0)
				printf("Failed to create file\n");

	//Check what type of file is being opened/created and determine what prgram to use when opening
	//	start out treating all files a .txt file or .jpg		

}

int close_file(char *file_to_close)
{
	fclose(file_to_close);
	//Not really sure i need to do error checking here for fclose
	//

}

int write_to_file(char *file_to_write, char *parent_directory, char *mode )
{
	//Open file and error check...
	err = open_file(file_to_write, parent_directory);
			if(error)
				return 0;
			else(file opened correctly, now write)
				//take input from user and store in buffer...
				fgets(user_input[512])
				if(mode is set to append)
					//write user_input to end of file
					fwrite(file_to_write,user_input, a+);
				else(mode is set to write)
					//overwrite file with user_input
					fwrite(file_to_write,user_input, w+);


}

int read_file(char *file_to_read, char *mode)
{
	//open file using previous open function
	open_file(file_to_read);
	//After the file is open, push it to stdio
	puts(file_to_read);

}




int main()
{
FILE *fp = fopen("disk", )

return 1;
}