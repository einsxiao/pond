#ifndef OSModule_Eins_H
#define OSModule_Eins_H

namespace pond{
  class OSModule: public Module{
    /////////////////////////////////
  public:
    OSModule(); ~OSModule();
    /////////////////////////////////
    /* static int Import(std::string filename,bool willBroken=false); */
    /////////////////////////////////
    /* int PD_Get(Object&); */
    /* int PD_Import(Object&); */
    int PD_Put(Object&);
    int PD_Export(Object&);
    /////////////////////////////////
    int PD_ReadList(Object&);
    /////////////////////////////////
    int PD_FileExistQ(Object&);
    int PD_Directory(Object&);
    int PD_SetDirectory(Object&);
    int PD_ParentDirectory(Object&);
    int PD_SetToParentDirectory(Object&);
    int PD_CreateDirectory(Object&);
    int PD_DeleteDirectory(Object&);
    /////////////////////////////////
    int PD_TouchFile(Object&);
    int PD_RenameFile(Object&);
    int PD_CopyFile(Object&);
    int PD_DeleteFile(Object&);
    int PD_FileBaseName(Object&);
    int PD_FileDirectoryName(Object&);
    int PD_FileNameJoin(Object&);
    int PD_FileNameSplit(Object&);
    /////////////////////////////////
    std::map<std::string, std::fstream*> fileTable;
    int PD_File(Object&);
    /////////////////////////////////
    int PD_GetEnv(Object&);
    int PD_SetEnv(Object&);


  };
};


#endif
