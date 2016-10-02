#ifndef GUI_H
#define GUI_H
////////////////////////////////////////////////////////////////////////////////////
// GUI.h - Graphical User Interface that allows user to interact with repository  //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*

Module Operations:
==================
This class defines the layout of the GUI and has event handlers to interact with the
server.

Required Files:
===============
GUI.h, GUI.cpp,
MockChannel.h, MockChannel.cpp,
HttpMessage.h, HttpMessage.cpp

Build Command:
==============
Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project GUI/GUI.vcxproj

Maintenance History:
====================
ver 1.0 : 2 May 2016 - first release

*/
#include <iostream>
#include <sstream>
#include <string>
#include <msclr\marshal_cppstd.h>
#include "../MockChannel/MockChannel.h"
#include "../HttpMessage/HttpMessage.h"


namespace GUIForm {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  using namespace System::Text;
  using namespace System::Windows;
  using namespace System::Windows::Input;
  using namespace System::Windows::Markup;
  using namespace System::Windows::Media;                   // TextBlock formatting
  using namespace System::Windows::Controls;                // TabControl
  using namespace System::Windows::Controls::Primitives;    // StatusBar
  using namespace System::Threading;
  using namespace System::Threading::Tasks;
  using namespace System::Windows::Threading;
  using namespace std;
  using namespace msclr::interop;

  /// <summary>
  /// Summary for GUI
  /// </summary>
  public ref class GUI : public System::Windows::Forms::Form
  {
  public:
    GUI(void)
    {
      InitializeComponent();
      //
      //TODO: Add the constructor code here
      //
      ObjectFactory* pObjFact = new ObjectFactory;
      pSendr_ = pObjFact->createSendr();
      pRecvr_ = pObjFact->createRecvr();
      pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
      pChann_->start();
      delete pObjFact;


    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~GUI()
    {
      //recvThread->Abort();
      //pChann_->stop();
      delete pChann_;
      delete pSendr_;
      delete pRecvr_;

      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::TabControl^  tabControl1;
  protected:
  private: System::Windows::Forms::TabPage^  tabPage1;
  private: System::Windows::Forms::TabPage^  tabPage2;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBox1;
  private: System::Windows::Forms::ListBox^  listBox1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBox2;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::RadioButton^  radioButton2;
  private: System::Windows::Forms::RadioButton^  radioButton1;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::Button^  button1;
  private: System::Windows::Forms::ListBox^ listBox2;
  private: System::Windows::Forms::FolderBrowserDialog^ FolderBrowserDialog1 = gcnew Forms::FolderBrowserDialog();
  private: ISendr* pSendr_;
  private: IRecvr* pRecvr_;
  private: IMockChannel* pChann_;


           //private:         Thread^ recvThread;
           /*public: Dispatcher dispatcher;*/
  private: System::Windows::Forms::Button^  button2;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::RadioButton^  radioButton4;
  private: System::Windows::Forms::RadioButton^  radioButton3;
  private: System::Windows::Forms::Button^  button3;
  private: System::Windows::Forms::Label^  label8;
  private: System::Windows::Forms::ListBox^  listBox3;
  private: System::Windows::Forms::Label^  label9;
  private: System::Windows::Forms::Button^  button4;


  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
      this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->button2 = (gcnew System::Windows::Forms::Button());
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBox2 = (gcnew System::Windows::Forms::TextBox());
      this->textBox1 = (gcnew System::Windows::Forms::TextBox());
      this->listBox1 = (gcnew System::Windows::Forms::ListBox());
      this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
      this->button4 = (gcnew System::Windows::Forms::Button());
      this->label9 = (gcnew System::Windows::Forms::Label());
      this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
      this->button3 = (gcnew System::Windows::Forms::Button());
      this->label8 = (gcnew System::Windows::Forms::Label());
      this->listBox3 = (gcnew System::Windows::Forms::ListBox());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->listBox2 = (gcnew System::Windows::Forms::ListBox());
      this->tabControl1->SuspendLayout();
      this->tabPage1->SuspendLayout();
      this->tabPage2->SuspendLayout();
      this->SuspendLayout();
      // 
      // tabControl1
      // 
      this->tabControl1->Controls->Add(this->tabPage1);
      this->tabControl1->Controls->Add(this->tabPage2);
      this->tabControl1->Location = System::Drawing::Point(-1, 0);
      this->tabControl1->Name = L"tabControl1";
      this->tabControl1->SelectedIndex = 0;
      this->tabControl1->Size = System::Drawing::Size(657, 470);
      this->tabControl1->TabIndex = 0;
      // 
      // tabPage1
      // 
      this->tabPage1->BackColor = System::Drawing::Color::LightGray;
      this->tabPage1->Controls->Add(this->label5);
      this->tabPage1->Controls->Add(this->button2);
      this->tabPage1->Controls->Add(this->button1);
      this->tabPage1->Controls->Add(this->radioButton2);
      this->tabPage1->Controls->Add(this->radioButton1);
      this->tabPage1->Controls->Add(this->label6);
      this->tabPage1->Controls->Add(this->label2);
      this->tabPage1->Controls->Add(this->label4);
      this->tabPage1->Controls->Add(this->label3);
      this->tabPage1->Controls->Add(this->label1);
      this->tabPage1->Controls->Add(this->textBox2);
      this->tabPage1->Controls->Add(this->textBox1);
      this->tabPage1->Controls->Add(this->listBox1);
      this->tabPage1->Location = System::Drawing::Point(4, 22);
      this->tabPage1->Name = L"tabPage1";
      this->tabPage1->Padding = System::Windows::Forms::Padding(3);
      this->tabPage1->Size = System::Drawing::Size(649, 444);
      this->tabPage1->TabIndex = 0;
      this->tabPage1->Text = L"Check-In";
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(305, 412);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(112, 13);
      this->label5->TabIndex = 6;
      this->label5->Text = L"                                   ";
      this->label5->TextAlign = System::Drawing::ContentAlignment::TopCenter;
      this->label5->Click += gcnew System::EventHandler(this, &GUI::label5_Click);
      // 
      // button2
      // 
      this->button2->Location = System::Drawing::Point(437, 169);
      this->button2->Name = L"button2";
      this->button2->Size = System::Drawing::Size(123, 25);
      this->button2->TabIndex = 5;
      this->button2->Text = L"Get Packages";
      this->button2->UseVisualStyleBackColor = true;
      this->button2->Click += gcnew System::EventHandler(this, &GUI::GetPackages);
      // 
      // button1
      // 
      this->button1->Location = System::Drawing::Point(284, 386);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(75, 23);
      this->button1->TabIndex = 4;
      this->button1->Text = L"Check-In";
      this->button1->UseVisualStyleBackColor = true;
      this->button1->Click += gcnew System::EventHandler(this, &GUI::CheckInPackage);
      // 
      // radioButton2
      // 
      this->radioButton2->AutoSize = true;
      this->radioButton2->Location = System::Drawing::Point(374, 145);
      this->radioButton2->Name = L"radioButton2";
      this->radioButton2->Size = System::Drawing::Size(51, 17);
      this->radioButton2->TabIndex = 3;
      this->radioButton2->TabStop = true;
      this->radioButton2->Text = L"Open";
      this->radioButton2->UseVisualStyleBackColor = true;
      // 
      // radioButton1
      // 
      this->radioButton1->AutoSize = true;
      this->radioButton1->Location = System::Drawing::Point(212, 145);
      this->radioButton1->Name = L"radioButton1";
      this->radioButton1->Size = System::Drawing::Size(57, 17);
      this->radioButton1->TabIndex = 3;
      this->radioButton1->TabStop = true;
      this->radioButton1->Text = L"Closed";
      this->radioButton1->UseVisualStyleBackColor = true;
      this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &GUI::radioButton1_CheckedChanged);
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(80, 145);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(75, 13);
      this->label6->TabIndex = 2;
      this->label6->Text = L"Check-in type:";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(80, 110);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(129, 13);
      this->label2->TabIndex = 2;
      this->label2->Text = L"Select Package directory:";
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(80, 178);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(112, 13);
      this->label4->TabIndex = 2;
      this->label4->Text = L"Select Dependencies:";
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(249, 24);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(110, 13);
      this->label3->TabIndex = 2;
      this->label3->Text = L"PACKAGE CHECK-IN";
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(80, 65);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(112, 13);
      this->label1->TabIndex = 2;
      this->label1->Text = L"Enter Package Name:";
      // 
      // textBox2
      // 
      this->textBox2->Location = System::Drawing::Point(212, 107);
      this->textBox2->Name = L"textBox2";
      this->textBox2->Size = System::Drawing::Size(349, 20);
      this->textBox2->TabIndex = 1;
      this->textBox2->Click += gcnew System::EventHandler(this, &GUI::SelectDirectory_Click);
      // 
      // textBox1
      // 
      this->textBox1->Location = System::Drawing::Point(212, 62);
      this->textBox1->Name = L"textBox1";
      this->textBox1->Size = System::Drawing::Size(349, 20);
      this->textBox1->TabIndex = 1;
      this->textBox1->TextChanged += gcnew System::EventHandler(this, &GUI::CheckInPkgName);
      // 
      // listBox1
      // 
      this->listBox1->FormattingEnabled = true;
      this->listBox1->Location = System::Drawing::Point(83, 194);
      this->listBox1->Name = L"listBox1";
      this->listBox1->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
      this->listBox1->Size = System::Drawing::Size(478, 186);
      this->listBox1->TabIndex = 0;
      this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &GUI::listBox1_SelectedIndexChanged);
      // 
      // tabPage2
      // 
      this->tabPage2->BackColor = System::Drawing::Color::LightGray;
      this->tabPage2->Controls->Add(this->button4);
      this->tabPage2->Controls->Add(this->label9);
      this->tabPage2->Controls->Add(this->radioButton4);
      this->tabPage2->Controls->Add(this->radioButton3);
      this->tabPage2->Controls->Add(this->button3);
      this->tabPage2->Controls->Add(this->label8);
      this->tabPage2->Controls->Add(this->listBox3);
      this->tabPage2->Controls->Add(this->label7);
      this->tabPage2->Location = System::Drawing::Point(4, 22);
      this->tabPage2->Name = L"tabPage2";
      this->tabPage2->Padding = System::Windows::Forms::Padding(3);
      this->tabPage2->Size = System::Drawing::Size(649, 444);
      this->tabPage2->TabIndex = 1;
      this->tabPage2->Text = L"Check-Out";
      // 
      // button4
      // 
      this->button4->Location = System::Drawing::Point(465, 65);
      this->button4->Name = L"button4";
      this->button4->Size = System::Drawing::Size(122, 23);
      this->button4->TabIndex = 8;
      this->button4->Text = L"Get Packages";
      this->button4->UseVisualStyleBackColor = true;
      this->button4->Click += gcnew System::EventHandler(this, &GUI::GetPackagesCheckOut);
      // 
      // label9
      // 
      this->label9->AutoSize = true;
      this->label9->Location = System::Drawing::Point(251, 404);
      this->label9->Name = L"label9";
      this->label9->Size = System::Drawing::Size(130, 13);
      this->label9->TabIndex = 7;
      this->label9->Text = L"                                         ";
      this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
      // 
      // radioButton4
      // 
      this->radioButton4->AutoSize = true;
      this->radioButton4->Location = System::Drawing::Point(427, 318);
      this->radioButton4->Name = L"radioButton4";
      this->radioButton4->Size = System::Drawing::Size(117, 17);
      this->radioButton4->TabIndex = 6;
      this->radioButton4->TabStop = true;
      this->radioButton4->Text = L"With dependencies";
      this->radioButton4->UseVisualStyleBackColor = true;
      // 
      // radioButton3
      // 
      this->radioButton3->AutoSize = true;
      this->radioButton3->Location = System::Drawing::Point(136, 318);
      this->radioButton3->Name = L"radioButton3";
      this->radioButton3->Size = System::Drawing::Size(132, 17);
      this->radioButton3->TabIndex = 5;
      this->radioButton3->TabStop = true;
      this->radioButton3->Text = L"Without dependencies";
      this->radioButton3->UseVisualStyleBackColor = true;
      // 
      // button3
      // 
      this->button3->Location = System::Drawing::Point(273, 362);
      this->button3->Name = L"button3";
      this->button3->Size = System::Drawing::Size(81, 22);
      this->button3->TabIndex = 4;
      this->button3->Text = L"Check-Out";
      this->button3->UseVisualStyleBackColor = true;
      this->button3->Click += gcnew System::EventHandler(this, &GUI::CheckOutPkg);
      // 
      // label8
      // 
      this->label8->AutoSize = true;
      this->label8->Location = System::Drawing::Point(77, 70);
      this->label8->Name = L"label8";
      this->label8->Size = System::Drawing::Size(91, 13);
      this->label8->TabIndex = 2;
      this->label8->Text = L"Choose package:";
      // 
      // listBox3
      // 
      this->listBox3->FormattingEnabled = true;
      this->listBox3->Location = System::Drawing::Point(80, 88);
      this->listBox3->Name = L"listBox3";
      this->listBox3->Size = System::Drawing::Size(508, 186);
      this->listBox3->TabIndex = 1;
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(259, 34);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(122, 13);
      this->label7->TabIndex = 0;
      this->label7->Text = L"PACKAGE CHECK-OUT";
      // 
      // listBox2
      // 
      this->listBox2->Location = System::Drawing::Point(0, 0);
      this->listBox2->Name = L"listBox2";
      this->listBox2->Size = System::Drawing::Size(120, 96);
      this->listBox2->TabIndex = 0;
      // 
      // GUI
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(657, 466);
      this->Controls->Add(this->tabControl1);
      this->Name = L"GUI";
      this->Text = L"GUI";

      this->tabControl1->ResumeLayout(false);
      this->tabPage1->ResumeLayout(false);
      this->tabPage1->PerformLayout();
      this->tabPage2->ResumeLayout(false);
      this->tabPage2->PerformLayout();
      this->ResumeLayout(false);
      this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &GUI::GUIFormClosing);


    }
#pragma endregion
  private:  std::string toStdString(String^ pStr) {
    std::string dst;
    for (int i = 0; i < pStr->Length; ++i)
      dst += (char)pStr[i];
    return dst;
  }

  private: System::Void SelectDirectory_Click(System::Object^  sender, System::EventArgs^  e) {
    FolderBrowserDialog1->ShowNewFolderButton = false;
    FolderBrowserDialog1->SelectedPath = System::IO::Directory::GetCurrentDirectory();
    std::cout << "\n  Browsing for folder";
    listBox2->Items->Clear();
    System::Windows::Forms::DialogResult result;
    result = FolderBrowserDialog1->ShowDialog();
    if (result == System::Windows::Forms::DialogResult::OK)
    {
      String^ path = FolderBrowserDialog1->SelectedPath;
      textBox2->Text = path;


    }
  }
  private: System::Void CheckInPkgName(System::Object^  sender, System::EventArgs^  e) {

  }



  private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
  }
  private: System::Void CheckInPackage(System::Object^  sender, System::EventArgs^  e) {

    if (textBox1->Text == "")
    {
      label5->Text = "Please enter a package Name";
      return;
    }
    else if (textBox2->Text == "")
    {
      label5->Text = "Please select location of package to check in";
      return;
    }
    else if (!radioButton1->Checked && !radioButton2->Checked) {
      label5->Text = "Please select check in type";
      return;
    }

    HttpMessage msg;
    msg.addAttribute("Command", "CheckIn");
    msg.addAttribute("ToAddr", "127.0.0.1:8080");
    msg.addAttribute("FromAddr", "127.0.0.1:8081");
    msg.addAttribute("Mode", "OneWay");
    string loc = marshal_as<std::string>(textBox2->Text);
    msg.addAttribute("Package-Location", loc);
    string name = (marshal_as<std::string>(textBox1->Text));
    msg.addAttribute("Package-Name", name);
    if (radioButton1->Checked) {
      msg.addAttribute("Check-in", "closed");
    }
    else {
      msg.addAttribute("Check-in", "open");
    }
    for each (auto item in listBox1->SelectedItems)
    {
      string dep = (marshal_as<std::string>(item->ToString()));
      msg.addAttribute("Dependencies", dep);
    }

    pSendr_->postMessage(msg);

    msg = pRecvr_->getMessage();

    std::string cmd = msg.findValue("Command");
    if (cmd == "CheckInDone") {
      label5->Text = "Check In complete";
    }
  }
  private: System::Void GetPackages(System::Object^  sender, System::EventArgs^  e) {
    HttpMessage msg;
    msg.addAttribute("Command", "GetPackages");
    msg.addAttribute("ToAddr", "127.0.0.1:8080");
    msg.addAttribute("FromAddr", "127.0.0.1:8081");
    msg.addAttribute("Mode", "OneWay");
    //msg.addContentLength();
    pSendr_->postMessage(msg);

    msg = pRecvr_->getMessage();
    std::string res = msg.getBody();
    displayPkgs(marshal_as<String^>(res));
  }
  private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
  }

  private: System::Void displayPkgs(String^ msg) {

    std::cout << "\n\nRecieved packages:\n";

    listBox1->Items->Clear();
    cli::array<String^>^ StringArray = msg->Split('\n');

    for each(String^ temp in StringArray) {
      listBox1->Items->Add(temp);
    }
  }

  private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
  }

  private: System::Void GetPackagesCheckOut(System::Object^  sender, System::EventArgs^  e) {
    HttpMessage msg;
    msg.addAttribute("Command", "GetPackagesChkOut");
    msg.addAttribute("ToAddr", "127.0.0.1:8080");
    msg.addAttribute("FromAddr", "127.0.0.1:8081");
    msg.addAttribute("Mode", "OneWay");
    //msg.addContentLength();
    pSendr_->postMessage(msg);

    msg = pRecvr_->getMessage();
    if (msg.findValue("Command") == "PackageListChkOut") {
      std::string res = msg.getBody();
      displayPkgs2(marshal_as<String^>(res));
    }
  }
  private: System::Void displayPkgs2(String^ msg) {

    std::cout << "\n\nRecieved packages:\n";

    listBox3->Items->Clear();
    cli::array<String^>^ StringArray = msg->Split('\n');

    for each(String^ temp in StringArray) {
      listBox3->Items->Add(temp);
    }
  }
  private: System::Void CheckOutPkg(System::Object^  sender, System::EventArgs^  e) {
    if (!listBox3->SelectedItem)
    {
      label9->Text = "Please select a package to check out";
      return;
    }

    else if (!radioButton3->Checked && !radioButton4->Checked) {
      label9->Text = "Please select check out type";
      return;
    }

    HttpMessage msg;
    msg.addAttribute("Command", "CheckOut");
    msg.addAttribute("ToAddr", "127.0.0.1:8080");
    msg.addAttribute("FromAddr", "127.0.0.1:8081");
    msg.addAttribute("Mode", "OneWay");
    msg.addAttribute("Package-Name", marshal_as<std::string>(listBox3->SelectedItem->ToString()));
    if (radioButton3->Checked) {
      msg.addAttribute("Dependencies", "False");
    }
    else {
      msg.addAttribute("Dependencies", "True");
    }
    pSendr_->postMessage(msg);

    msg = pRecvr_->getMessage();

    std::string cmd = msg.findValue("Command");
    if (cmd == "CheckOut") {
      label9->Text = "Check Out complete";
    }
  }


  private:void GUIFormClosing(System::Object^ sender, FormClosingEventArgs^ e) {
    HttpMessage msg;
    msg.addBody("");
    msg.addAttribute("Command", "quit");
    msg.addAttribute("ToAddr", "127.0.0.1:8080");
    msg.addAttribute("FromAddr", "127.0.0.1:8081");
    msg.addAttribute("Mode", "OneWay");
    pSendr_->postMessage(msg);
    msg = pRecvr_->getMessage();
  }
  };
  }
#endif