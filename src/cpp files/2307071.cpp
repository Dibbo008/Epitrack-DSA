// 2307071.cpp
#include "shared.h"

void handle_2307071(int choice, Controller& ctrl) {
    switch (choice) {

    // ===== 6. EMERGENCY QUEUE OPERATIONS  [MaxHeap] =====
    case 6: {
        int sub;
        cout << "\n  --- Emergency Triage (Max-Heap) ---\n";
        cout << "  1. Add Critical Patient\n";
        cout << "  2. Dispatch Highest Priority Patient\n";
        cout << "  3. Display Emergency Queue\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        int hid;
        cout << "  Hospital ID: "; cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        if (sub == 1) {
            int sev; string pname;
            cout << "  Patient name: ";     getline(cin,pname);
            cout << "  Severity (1-100): "; cin >> sev; cin.ignore(1000, '\n');
            h->addCritical(sev, pname);
        } else if (sub == 2) {
            h->dispatchCritical();
        } else if (sub == 3) {
            h->showICU();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 7. VACCINATION CENTER QUEUE OPERATIONS  [TemplateQueue + TemplateStack] =====
    case 7: {
        int sub;
        cout << "\n  --- Vaccination Center Queue ---\n";
        cout << "  1. Enqueue Citizen for Vaccination\n";
        cout << "  2. Vaccinate Next Citizen\n";
        cout << "  3. Display Vaccination Queue\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        int vid;
        cout << "  Vaccine Center ID: "; cin >> vid; cin.ignore(1000, '\n');
        VaccineCenter* vc = ctrl.getcenter(vid);
        if (!vc) break;
        if (sub == 1) {
            string name;
            cout << "  Citizen name: "; getline(cin,name);
            vc->enqueueCitizen(name);
        } else if (sub == 2) {
            int day;
            cout << "  Vaccination day (1-365): "; cin >> day; cin.ignore(1000, '\n');
            vc->vaccinateNext(day);
        } else if (sub == 3) {
            vc->showQueue();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 13. VACCINATION ANALYTICS  [dailyVacc array + range query] =====
    case 13: {
        int sub;
        cout << "\n  --- Vaccination Analytics ---\n";
        cout << "  1. Record Daily Vaccinations (Bulk)\n";
        cout << "  2. Query Vaccinations by Date Range\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            int vid, day, cnt;
            cout << "  Vaccine Center ID: ";      cin >> vid; cin.ignore(1000, '\n');
            VaccineCenter* vc = ctrl.getcenter(vid);
            if (!vc) break;
            cout << "  Day (1-365): ";            cin >> day; cin.ignore(1000, '\n');
            cout << "  Vaccinations to record: "; cin >> cnt; cin.ignore(1000, '\n');
            for (int k = 0; k < cnt; k++) {
                vc->enqueueCitizen("BulkRecord_" + to_string(k));
                vc->vaccinateNext(day);
            }
            cout << GREEN << "  Recorded " << cnt << " vaccinations for center "
                 << vc->getName() << " on day " << day << ".\n" << RESET;
        } else if (sub == 2) {
            int l, r;
            cout << "  From day: "; cin >> l; cin.ignore(1000, '\n');
            cout << "  To day:   "; cin >> r; cin.ignore(1000, '\n');
            ctrl.queryCases(l, r);
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 15. AUDIT LOG & UNDO  [TemplateStack] =====
    case 15: {
        int sub;
        cout << "\n  --- Audit Log & Undo ---\n";
        cout << "  1. View Global Audit Logs\n";
        cout << "  2. Investigate Last Operation\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            ctrl.showAudit();
        } else if (sub == 2) {
            ctrl.undo();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    default:
        break;
    }
}