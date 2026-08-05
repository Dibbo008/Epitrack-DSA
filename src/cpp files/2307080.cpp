// 2307080.cpp
#include "shared.h"

void handle_2307080(int choice, Controller& ctrl) {
    switch (choice) {

    // ===== 1. REGISTER HOSPITAL =====
    case 1: {
        string name; int beds, icu;
        cout << "  Hospital name: ";  getline(cin,name);
        cout << "  Total beds: ";    cin >> beds; cin.ignore(1000, '\n');
        cout << "  ICU beds: ";      cin >> icu; cin.ignore(1000, '\n');
        ctrl.registerHospital(name, beds, icu);
        break;
    }

    // ===== 2. HOSPITAL & VACCINE CENTER MANAGEMENT =====
    case 2: {
        int sub;
        cout << "\n  --- Hospital & Vaccine Center Management ---\n";
        cout << "  1. Display All Hospitals\n";
        cout << "  2. Register Vaccine Center\n";
        cout << "  3. Display All Vaccine Centers\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            ctrl.displayAllHospitals();
        } else if (sub == 2) {
            int id; string name;
            cout << "  Center ID: ";   cin >> id; cin.ignore(1000, '\n');
            cout << "  Center Name: "; getline(cin,name);
            ctrl.registerCenter(id, name);
        } else if (sub == 3) {
            ctrl.displayAllCenters();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 3. REGISTER PATIENT  [BST insert] =====
    case 3: {
        int hid, pid, sev; string pname;
        cout << "  Hospital ID: ";     cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        cout << "  Patient ID: ";      cin >> pid; cin.ignore(1000, '\n');
        cout << "  Patient name: ";    getline(cin,pname);
        cout << "  Severity (1-10): "; cin >> sev; cin.ignore(1000, '\n');
        h->addPatient(pid, pname, sev);
        break;
    }

    // ===== 4. DISCHARGE PATIENT  [BST delete] =====
    case 4: {
        int hid, pid;
        cout << "  Hospital ID: ";              cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        cout << "  Patient ID to discharge: ";  cin >> pid; cin.ignore(1000, '\n');
        h->removePatient(pid);
        break;
    }

    // ===== 5. SEARCH / DISPLAY PATIENTS  [BST search / inorder] =====
    case 5: {
        int sub;
        cout << "\n  --- Patient Lookup ---\n";
        cout << "  1. Search Patient by ID\n";
        cout << "  2. Display All Patients in Hospital\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        int hid;
        cout << "  Hospital ID: "; cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        if (sub == 1) {
            int pid;
            cout << "  Patient ID: "; cin >> pid; cin.ignore(1000, '\n');
            h->searchPatient(pid);
        } else if (sub == 2) {
            cout << "\n--- Patients in " << h->getName() << " ---\n";
            h->showPatients();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 14. VACCINATION ANALYTICS DASHBOARD  [QuickSort + BinarySearch] =====
    case 14: {
        int n = ctrl.getCenterCount();
        if (n == 0) { cout << RED << "  No vaccine centers registered.\n" << RESET; break; }

        int sub;
        cout << "\n===== VACCINATION ANALYTICS DASHBOARD =====\n";
        cout << "  1. Rank Centers by Total Vaccinations (QuickSort, descending)\n";
        cout << "  2. Search Center by ID (Binary Search)\n";
        cout << "  3. View Center Audit Log\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');

        CenterReport reports1[100], reports2[100];
        for (int i = 0; i < n; i++) {
            VaccineCenter* vc = ctrl.getCenterByIndex(i);
            reports1[i] = CenterReport(vc->getid(), vc->getTotalVaccinations());
            reports2[i] = CenterReport(vc->getid(), vc->getTotalVaccinations());
        }

        quickSortByVacc(reports1, 0, n-1);
        quickSortByID(reports2, 0, n-1);

        if (sub == 1) {
            cout << CYAN << "\n--- Centers Ranked by Total Vaccinations ---\n" << RESET;
            for (int i = 0; i < n; i++)
                cout << "  Rank " << (i+1)
                     << " | Center ID: "          << reports1[i].centerID
                     << " | Total Vaccinations: " << reports1[i].totalVaccinations << "\n";
        } else if (sub == 2) {
            int targetID;
            cout << "  Enter Center ID to search: "; cin >> targetID; cin.ignore(1000, '\n');
            int idx = binarySearchByID(reports2, n, targetID);
            if (idx == -1)
                cout << RED << "  Center ID " << targetID << " not found.\n" << RESET;
            else
                cout << GREEN << "\n  Found Center:\n  ID: " << reports2[idx].centerID
                     << " | Total Vaccinations: " << reports2[idx].totalVaccinations << "\n" << RESET;
        } else if (sub == 3) {
            int vid;
            cout << "  Center ID: "; cin >> vid; cin.ignore(1000, '\n');
            VaccineCenter* vc = ctrl.getcenter(vid);
            if (vc) vc->showAudit();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    default:
        break;
    }
}