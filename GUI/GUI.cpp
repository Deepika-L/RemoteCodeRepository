/////////////////////////////////////////////////////////////////////////////////////
// GUI.cpp - Graphical User Interface that allows user to interact with repository //
// ver 1.0                                                                         //
//                                                                                 //
// Application: Project 4 for CIS 687, Spring 2016                                 //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                    //
// Author:      Deepika Lakshmanan                                                 //
//              dlakshma@syr.edu                                                   //
/////////////////////////////////////////////////////////////////////////////////////
#include "GUI.h"
using namespace GUIForm;

//------------------------------------Test stub--------------------------------------
#ifdef TEST_GUI
[STAThreadAttribute]
int main()
{
  GUIForm::GUI form;
  form.ShowDialog();
  
  return 0;
}

#endif