#pragma once
#include <iostream>
#include <string>
#include "../CommandShell/Dispatcher/CommandDispatcher.h"
namespace AFL_forms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for AFL_main_form
	/// </summary>
	public ref class AFL_main_form : public System::Windows::Forms::Form
	{
	public:
		AFL_main_form(void)
		{
			dispatcher = new CommandDispatcher();
			InitializeComponent();
			
			//
			//TODO: Add the constructor code here
			//
		}
	private: System::Windows::Forms::RichTextBox^  richTextBox3;
	public:
		CommandDispatcher* dispatcher;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AFL_main_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	protected:
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::RichTextBox^  richTextBox2;
	private: System::Windows::Forms::Button^  button1;

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

			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->richTextBox3 = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(12, 451);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(614, 20);
			this->textBox1->TabIndex = 0;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 30);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(614, 392);
			this->richTextBox1->TabIndex = 1;
			this->richTextBox1->Text = L"";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(648, 30);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(360, 190);
			this->richTextBox2->TabIndex = 2;
			this->richTextBox2->Text = gcnew String(dispatcher->env.toStringFSA().c_str());
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(648, 451);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Submit";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &AFL_main_form::button1_Click);
			// 
			// richTextBox3
			// 
			this->richTextBox3->Location = System::Drawing::Point(648, 248);
			this->richTextBox3->Name = L"richTextBox3";
			this->richTextBox3->Size = System::Drawing::Size(360, 174);
			this->richTextBox3->TabIndex = 4;
			this->richTextBox3->Text = gcnew String(dispatcher->env.toStringNPDA().c_str());
			// 
			// AFL_main_form
			// 
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(AFL_main_form::typeid));
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1020, 517);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Controls->Add(this->richTextBox3);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->textBox1);
			this->Name = L"AFL_main_form";
			this->Text = L"AFL";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void MarshalString(String ^ s, std::string& os) {
			using namespace Runtime::InteropServices;
			const char* chars =
				(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	
		std::string input;
		MarshalString(textBox1->Text, input);
		std::string resFromDispatcher = dispatcher->dispatch(input);
		
		std::string currentConsole;
		MarshalString(richTextBox1->Text, currentConsole);
		currentConsole += "> " + input + '\n';
		currentConsole +=  resFromDispatcher + '\n';
		richTextBox1->Text = gcnew String(currentConsole.c_str());

		std::string envOutput1 = dispatcher->env.toStringFSA();
		richTextBox2->Text = gcnew String(envOutput1.c_str());
		
		std::string envOutput2 = dispatcher->env.toStringNPDA();
		richTextBox3->Text = gcnew String(envOutput2.c_str());
	}
	};
}
