#include "stdafx.h"
#include "MachineData.h"


MachineData::MachineData() {
  connection = gcnew SqlConnection(connection_string);
  connected = connect_to_db();
}

MachineData::~MachineData() {
  if (connected) {
    connection->Close();
    if (connection->State == System::Data::ConnectionState::Closed)
      connected = false;
    else
      connected = true;
  }
}

bool MachineData::connect_to_db() {
  try {
    if (connection != nullptr)
      connection->Open();
  }
  catch (System::InvalidCastException ^ioe) {
    System::Console::WriteLine(ioe->Message);
    throw ioe;
  }
  catch (System::Data::SqlClient::SqlException ^sqe)  {
    System::Console::WriteLine(sqe->Message);
    throw sqe;
  }
  catch (System::Exception ^e) {
    System::Console::WriteLine(e->Message);
    throw e;
  }

  if (connection->State == System::Data::ConnectionState::Open)
    return true;
  else
    return false;
}

int MachineData::get_machine_id(string ^machname) {
  int res = 0;
  if (connected) {
    string ^sql = "SELECT MACHID FROM CUT_MACHINES WHERE MACHNAME = @machname";
    SqlCommand ^comm = gcnew SqlCommand(sql, connection);
    comm->Parameters->AddWithValue("@machname", machname);
    DataReader ^dr;
    try {
      dr = comm->ExecuteReader(System::Data::CommandBehavior::SingleResult);

      if (dr->Read()) {
        res = dr->GetInt32(0);
        dr->Close();
        return res;
      }

    }
    catch (System::Exception ^e) {
      System::Console::WriteLine(e->Message);
      throw e;
    }
    dr->Close();
  }
  return res;
}

DataTable^ MachineData::get_where_used(string ^s) {
  if (connected) {
    string ^sql = "SELECT * FROM CUT_PARTS WHERE CNC1 = @cnc1 OR CNC2 = @cnc2";
    SqlCommand ^comm = gcnew SqlCommand(sql, connection);

    comm->Parameters->AddWithValue("@cnc1", s);
    comm->Parameters->AddWithValue("@cnc2", s);
    DataAdapter ^da = gcnew DataAdapter(comm);
    DataSet ^ds = gcnew DataSet();
    da->Fill(ds);
    return ds->Tables[0];
  }
}

FileInfoList^ MachineData::get_program_list(string ^path) {
  System::IO::TextReader ^tr = gcnew System::IO::StreamReader(path);
  FileInfoList ^fil = gcnew FileInfoList();
  string ^line = string::Empty;
  while (true) {
    line = tr->ReadLine();
    if (line == nullptr) {
      break;
    }

    if (line->ToUpper()->EndsWith("CNC")) {
      fil->Add(gcnew FileInfo(line));
    }
    else {
      continue;
    }
  }
  return fil;
}

IntList^ MachineData::part_list(string ^s) {
  IntList ^il = gcnew IntList();
  if (connected) {
    for each (System::Data::DataRow ^r in get_where_used(s)->Rows) {
      il->Add((int)r->ItemArray[0]);
    }
  }
  return il;
}

CommandList^ MachineData::build_sqls(FileInfo ^fi, int machid, int priority) {
  CommandList ^sqls = gcnew CommandList();
  string ^sql = "INSERT INTO CUT_MACHINE_PROGRAMS (MACHID, PARTID, PRIORITY) VALUES (@machid, @partid, @prio)";
  array<wchar_t, 1> ^c = { '.' };
  string ^prog = fi->Name->Split(c)[0];
  int tempint = 0;

  for each (System::Data::DataRow ^r in get_where_used(prog)->Rows) {
    SqlCommand ^comm = gcnew SqlCommand(sql, connection);
    comm->Parameters->Add("@machid", machid);
    comm->Parameters->Add("@partid", parse<int>(r->ItemArray[0]->ToString()));
    comm->Parameters->Add("@prio", priority);
    sqls->Add(comm);
  }
  return sqls;
}

bool MachineData::record_exists(int machid, string ^partnum) {
  bool res = false;
  string ^sql = "SELECT CUT_MACHINES.MACHNAME FROM " +
    "(CUT_MACHINE_PROGRAMS INNER JOIN CUT_PARTS ON CUT_MACHINE_PROGRAMS.PARTID = CUT_PARTS.PARTID) " +
    "INNER JOIN CUT_MACHINES ON CUT_MACHINE_PROGRAMS.MACHID = CUT_MACHINES.MACHID " +
    "WHERE ((CUT_MACHINES.MACHID = @machid) AND (CUT_PARTS.PARTNUM = @partnum))";
  if (connected) {
    SqlCommand ^comm = gcnew SqlCommand(sql);
    comm->Parameters->Add("@machid", machid);
    comm->Parameters->Add("@partnum", partnum);
    try {
      DataReader ^dr = comm->ExecuteReader(System::Data::CommandBehavior::SingleResult);
      res = dr->Read();
      res = dr->HasRows;
      dr->Close();
    }
    catch (System::Exception ^e) {
      System::Console::WriteLine(e->Message);
    }
  }
  return res;
}

void MachineData::print_part_list(string ^s) {
  for each (int i in part_list(s)) {
    System::Console::WriteLine(string::Format("  {0}", i.ToString()));
  }
}

void MachineData::print_list(string ^s) {
  if (connected) {
    DataTable ^dt = get_where_used(s);
    for each (System::Object ^col in dt->Columns) {
      System::Console::Write(string::Format("{0} ", col->ToString()));
    }
    for each (System::Data::DataRow ^r in dt->Rows) {
      for each (System::Object ^o in r->ItemArray) {
        System::Console::Write(string::Format("{0} ", o->ToString()));
      }
      System::Console::WriteLine();
    }
  }
  else {
    System::Console::WriteLine("No connection.");
  }
}

void MachineData::print_program_list(string ^path) {
  FileInfo ^p = gcnew FileInfo(path);
  array<wchar_t, 1> ^c = { '.' };
  string ^prog = string::Empty;
  if (p->Exists) {
    for each (FileInfo ^fi in get_program_list(p->FullName)) {
      prog = fi->Name->Split(c)[0];
      System::Console::WriteLine(prog);
      print_part_list(prog);
    }
  }
}
