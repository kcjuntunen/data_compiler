#pragma once

typedef System::String string;
typedef System::Object object;
typedef System::Data::SqlClient::SqlConnection SqlConnection;
typedef System::Data::SqlClient::SqlCommand SqlCommand;
typedef System::Data::SqlClient::SqlDataAdapter DataAdapter;
typedef System::Data::SqlClient::SqlDataReader DataReader;
typedef System::Data::DataSet DataSet;
typedef System::Data::DataTable DataTable;
typedef System::Collections::Generic::List<string^> StringList;
typedef System::Collections::Generic::List<int> IntList;
typedef System::IO::FileInfo FileInfo;
typedef System::Collections::Generic::List<FileInfo^> FileInfoList; 
typedef System::Collections::Generic::List<SqlCommand^> CommandList;

ref class MachineData {
private:
  string ^connection_string = "Server=AMSTORE-SQL-05;Database=ENGINEERING;Trusted_Connection=True;";
  SqlConnection ^connection;
  bool connect_to_db();
public:
  bool connected = false;
  MachineData();
  ~MachineData();
  DataTable^ get_where_used(string ^s);
  IntList^ part_list(string ^s);
  FileInfoList^ get_program_list(string ^path);
  CommandList^ build_sqls(FileInfo ^fi, int machid, int priority);
  int get_machine_id(string ^machnum);
  bool record_exists(int machid, string ^partnum);

  template<typename T>
  T parse(string^ input) {
    T tp;
    bool res = T::TryParse(input, tp);
    return tp;
  }

  void print_list(string ^s);
  void print_part_list(string ^s);
  void print_program_list(string ^path);
};

