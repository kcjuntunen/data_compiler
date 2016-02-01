// compile_machine_data.cpp : main project file.

#include "stdafx.h"
#include "MachineData.h"

using namespace System;

int main(array<System::String ^> ^args)
{
  MachineData ^md = gcnew MachineData();
  int count = 0;
  if (args->Length > 1) {
    int mid = md->get_machine_id(args[1]);
    FileInfoList ^fil = md->get_program_list(args[0]);
    for each (FileInfo ^fi in fil) {
      for each (SqlCommand ^c in md->build_sqls(fi, mid, 1)) {
        Console::Write(".");
        c->ExecuteNonQuery();
        count++;
      }
    }
    Console::WriteLine();
    Console::WriteLine(string::Format("{0} records inserted.", count));
  }
  else {
    System::Console::WriteLine("ARGS:");
    for each (string ^s in args) {
      System::Console::Write(string::Format("{0} args.", s));
    }
  }
  /*System::Console::ReadLine();*/
  return 0;
}
