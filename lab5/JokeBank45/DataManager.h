#pragma once
#include "DatabaseManager.h"

namespace JokeBank45 {

    using namespace System;
    using namespace System::Collections::Generic;

    public ref class DataManager sealed
    {
    private:
        static DataManager^ _instance = nullptr;

        DataManager()
        {
        }

    public:
        static property DataManager^ Instance
        {
            DataManager^ get()
            {
                if (_instance == nullptr)
                {
                    _instance = gcnew DataManager();
                }
                return _instance;
            }
        }

        void AddContributor(String^ name, int age)
        {
            DatabaseManager::Instance->AddContributor(name, age);
        }

        List<ManagedContributor^>^ GetAllContributors()
        {
            return DatabaseManager::Instance->GetAllContributors();
        }

        ManagedContributor^ GetContributor(int index)
        {
            List<ManagedContributor^>^ contributors = GetAllContributors();
            if (index >= 0 && index < contributors->Count)
                return contributors[index];
            return nullptr;
        }

        int GetContributorCount()
        {
            return DatabaseManager::Instance->GetContributorCount();
        }

        bool RemoveContributor(int index)
        {
            List<ManagedContributor^>^ contributors = GetAllContributors();
            if (index >= 0 && index < contributors->Count)
            {
                return DatabaseManager::Instance->DeleteContributor(contributors[index]->Id);
            }
            return false;
        }

        bool UpdateContributor(int index, String^ name, int age, double size)
        {
            List<ManagedContributor^>^ contributors = GetAllContributors();
            if (index >= 0 && index < contributors->Count)
            {
                return DatabaseManager::Instance->UpdateContributor(contributors[index]->Id, name, age, size);
            }
            return false;
        }
    };
}