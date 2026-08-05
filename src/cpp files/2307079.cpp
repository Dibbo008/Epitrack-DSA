// 2307079.cpp
#include "shared.h"

void handle_2307079(int choice, Controller& ctrl, ContactGraph& cGraph,
                    InfectedVaccQueue& ivq, LinkedList& vaccOrderList, SupplyChain& sc) {
    switch (choice) {

    // ===== 8. ADD CONTACT RELATIONSHIP  [Graph undirected] =====
    case 8: {
        int hid, a, b;
        cout << "  Hospital ID (for validation): "; cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        cout << "  Patient A ID: "; cin >> a; cin.ignore(1000, '\n');
        cout << "  Patient B ID: "; cin >> b; cin.ignore(1000, '\n');
        if (!h->patientExists(a)) {
            cout << RED << "  Patient ID " << a << " not found in hospital "
                 << h->getName() << ".\n" << RESET; break;
        }
        if (!h->patientExists(b)) {
            cout << RED << "  Patient ID " << b << " not found in hospital "
                 << h->getName() << ".\n" << RESET; break;
        }
        cGraph.addContact(a, b);
        cout << GREEN << "  Contact P" << a << " <-> P" << b << " recorded.\n" << RESET;
        break;
    }

    // ===== 9. RECORD DIRECTED EXPOSURE  [Graph directed] =====
    case 9: {
        int hid, a, b;
        cout << "  Hospital ID (for validation): "; cin >> hid; cin.ignore(1000, '\n');
        Hospital* h = ctrl.getHospital(hid);
        if (!h) break;
        cout << "  Exposer Patient ID: "; cin >> a; cin.ignore(1000, '\n');
        cout << "  Exposed Patient ID: "; cin >> b; cin.ignore(1000, '\n');
        if (!h->patientExists(a)) {
            cout << RED << "  Exposer Patient ID " << a << " not found in hospital "
                 << h->getName() << ".\n" << RESET; break;
        }
        if (!h->patientExists(b)) {
            cout << RED << "  Exposed Patient ID " << b << " not found in hospital "
                 << h->getName() << ".\n" << RESET; break;
        }
        cGraph.addExposure(a, b);
        cout << GREEN << "  Directed exposure recorded: P" << a << " --> P" << b << "\n" << RESET;
        break;
    }

    // ===== 10. GRAPH TRAVERSAL & SUPER SPREADERS  [BFS / DFS] =====
    case 10: {
        int sub;
        cout << "\n  --- Contact Tracing Graph Operations ---\n";
        cout << "  1. Trace Exposure\n";
        cout << "  2. Detect Infection Cluster\n";
        cout << "  3. Identify Super Spreaders\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            int start;
            cout << "  Start Patient ID: "; cin >> start; cin.ignore(1000, '\n');
            cGraph.BFS(start);
        } else if (sub == 2) {
            int start;
            cout << "  Start Patient ID: "; cin >> start; cin.ignore(1000, '\n');
            cGraph.DFS(start);
        } else if (sub == 3) {
            int threshold;
            cout << "  Minimum contacts threshold: "; cin >> threshold; cin.ignore(1000, '\n');
            cGraph.identifySuperSpreaders(threshold);
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 11. INFECTED VACC QUEUE OPERATIONS  [TopoSort + LinkedList + TemplateQueue] =====
    case 11: {
        int sub;
        cout << "\n  --- Infected Vaccination Queue (VaccQueue2) ---\n";
        cout << "  1. Generate Queue from Exposure Chain (Topo Sort)\n";
        cout << "  2. Display Infected Vaccination Queue\n";
        cout << "  3. Vaccinate Next Infected Patient\n";
        cout << "  4. Display Vaccination Order Linked List\n";
        cout << "  5. Show Topo Order (Print Only, no persist)\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            cout << CYAN << "\n  Building VaccQueue2 from Exposure Chain...\n" << RESET;
            ivq.buildFromGraph(cGraph, vaccOrderList);
        } else if (sub == 2) {
            ivq.display();
        } else if (sub == 3) {
            ivq.vaccinateNext(ctrl);
        } else if (sub == 4) {
            cout << "\n--- Vaccination Order Linked List ---\n  ";
            vaccOrderList.display();
        } else if (sub == 5) {
            cout << CYAN << "\n  Topological Sort (display only):\n" << RESET;
            LinkedList tmpList;
            TemplateQueue<int> tmpQ;
            cGraph.topoSort(tmpList, tmpQ);
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    // ===== 12. SUPPLY CHAIN OPERATIONS  [TopoSort] =====
    case 12: {
        int sub;
        cout << "\n  --- Supply Chain ---\n";
        cout << "  1. Add Supply Dependency\n";
        cout << "  2. Generate Distribution Order (Topo Sort)\n";
        cout << "  Choice: "; cin >> sub; cin.ignore(1000, '\n');
        if (sub == 1) {
            int u, v;
            cout << "  Nodes: 0=Manufacturer 1=RegWarehouse 2=DistStore\n";
            cout << "         3=HospitalA    4=HospitalB   5=Clinic\n";
            cout << "  From: "; cin >> u; cin.ignore(1000, '\n');
            cout << "  To:   "; cin >> v; cin.ignore(1000, '\n');
            sc.addEdge(u, v);
            cout << GREEN << "  Supply dependency added.\n" << RESET;
        } else if (sub == 2) {
            sc.addEdge(0,1); 
            sc.topoSort();
        } else {
            cout << RED << "  Invalid sub-choice.\n" << RESET;
        }
        break;
    }

    default:
        break;
    }
}