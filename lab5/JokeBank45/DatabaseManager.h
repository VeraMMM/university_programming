#pragma once

namespace JokeBank45 {

    using namespace System;
    using namespace System::Data;
    using namespace System::Data::SQLite;
    using namespace System::Collections::Generic;

    public ref class ManagedContributor
    {
    public:
        property int Id;
        property String^ Name;
        property int Age;
        property double Size;

        ManagedContributor(int id, String^ name, int age, double size)
        {
            Id = id;
            Name = name;
            Age = age;
            Size = size;
        }

        ManagedContributor(String^ name, int age)
        {
            Id = 0;
            Name = name;
            Age = age;
            Size = 0.0;
        }
    };

    public ref class DatabaseManager sealed
    {
    private:
        static DatabaseManager^ _instance = nullptr;
        SQLiteConnection^ _connection;
        String^ _databasePath;

        DatabaseManager()
        {
            _databasePath = "JokeBank.db";
            InitializeDatabase();
        }

    public:
        static property DatabaseManager^ Instance
        {
            DatabaseManager^ get()
            {
                if (_instance == nullptr)
                {
                    _instance = gcnew DatabaseManager();
                }
                return _instance;
            }
        }

        void InitializeDatabase()
        {
            try
            {
                String^ connectionString = String::Format("Data Source={0};Version=3;", _databasePath);
                _connection = gcnew SQLiteConnection(connectionString);
                _connection->Open();

                String^ createTableQuery =
                    "CREATE TABLE IF NOT EXISTS Contributors ("
                    "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "Name TEXT NOT NULL, "
                    "Age INTEGER NOT NULL, "
                    "Size REAL DEFAULT 0.0)";

                SQLiteCommand^ command = gcnew SQLiteCommand(createTableQuery, _connection);
                command->ExecuteNonQuery();

                System::Diagnostics::Debug::WriteLine("✅ Database initialized!");
            }
            catch (Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("❌ Database error: " + ex->Message);
            }
        }

        void AddContributor(String^ name, int age)
        {
            try
            {
                String^ query = "INSERT INTO Contributors (Name, Age, Size) VALUES (@name, @age, 0.0)";
                SQLiteCommand^ command = gcnew SQLiteCommand(query, _connection);
                command->Parameters->AddWithValue("@name", name);
                command->Parameters->AddWithValue("@age", age);
                command->ExecuteNonQuery();

                System::Diagnostics::Debug::WriteLine("✅ Contributor added: " + name);
            }
            catch (Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("❌ Error adding contributor: " + ex->Message);
            }
        }

        List<ManagedContributor^>^ GetAllContributors()
        {
            List<ManagedContributor^>^ contributors = gcnew List<ManagedContributor^>();

            try
            {
                String^ query = "SELECT * FROM Contributors ORDER BY Id";
                SQLiteCommand^ command = gcnew SQLiteCommand(query, _connection);
                SQLiteDataReader^ reader = command->ExecuteReader();

                while (reader->Read())
                {
                    ManagedContributor^ contributor = gcnew ManagedContributor(
                        reader->GetInt32(0),  // Id
                        reader->GetString(1), // Name  
                        reader->GetInt32(2),  // Age
                        reader->GetDouble(3)  // Size
                    );
                    contributors->Add(contributor);
                }
                reader->Close();
            }
            catch (Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("❌ Error getting contributors: " + ex->Message);
            }

            return contributors;
        }

        bool UpdateContributor(int id, String^ name, int age, double size)
        {
            try
            {
                String^ query = "UPDATE Contributors SET Name = @name, Age = @age, Size = @size WHERE Id = @id";
                SQLiteCommand^ command = gcnew SQLiteCommand(query, _connection);
                command->Parameters->AddWithValue("@name", name);
                command->Parameters->AddWithValue("@age", age);
                command->Parameters->AddWithValue("@size", size);
                command->Parameters->AddWithValue("@id", id);

                int rowsAffected = command->ExecuteNonQuery();
                System::Diagnostics::Debug::WriteLine("✅ Updated contributor: " + name + ", Age: " + age + ", Size: " + size);
                return rowsAffected > 0;
            }
            catch (Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("❌ Error updating contributor: " + ex->Message);
                return false;
            }
        }

        bool DeleteContributor(int id)
        {
            try
            {
                String^ query = "DELETE FROM Contributors WHERE Id = @id";
                SQLiteCommand^ command = gcnew SQLiteCommand(query, _connection);
                command->Parameters->AddWithValue("@id", id);

                return command->ExecuteNonQuery() > 0;
            }
            catch (Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("❌ Error deleting contributor: " + ex->Message);
                return false;
            }
        }

        int GetContributorCount()
        {
            try
            {
                String^ query = "SELECT COUNT(*) FROM Contributors";
                SQLiteCommand^ command = gcnew SQLiteCommand(query, _connection);
                return Convert::ToInt32(command->ExecuteScalar());
            }
            catch (Exception^ ex)
            {
                return 0;
            }
        }

        ~DatabaseManager()
        {
            if (_connection != nullptr && _connection->State == ConnectionState::Open)
            {
                _connection->Close();
                delete _connection;
            }
        }
    };
}