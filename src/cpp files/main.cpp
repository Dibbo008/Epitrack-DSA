// main.cpp
// EpiTrack Enterprise Pandemic Management System
// Connects: 2307080.cpp | 2307071.cpp | 2307079.cpp
// g++ main.cpp 2307071.cpp 2307079.cpp 2307080.cpp -o epitrack.exe
// 


#include "shared.h"
#include "2307080.h" 
#include "2307071.h"
#include "2307079.h"

// =====================================================================
//  COLOR DEFINITIONS  (defined once here, declared extern in shared.h)
// =====================================================================
string RESET  = "\033[0m";
string RED    = "\033[1;31m";
string GREEN  = "\033[1;32m";
string YELLOW = "\033[1;33m";
string CYAN   = "\033[1;36m";
string BLUE   = "\033[1;34m";

// =====================================================================
//  UTILITY
// =====================================================================
void printCentered(string text, int width) {
    int len = text.length();
    int pad = (width - len) / 2;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << text << endl;
}

void quickSortByID(CenterReport arr[], int low, int high) {
    if (low >= high) return;
    int pivot = arr[high].centerID, i = low - 1;
    for (int j = low; j < high; j++)
        if (arr[j].centerID < pivot) { i++; CenterReport tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp; }
    CenterReport tmp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = tmp;
    int pi = i + 1;
    quickSortByID(arr, low, pi - 1);
    quickSortByID(arr, pi + 1, high);
}

void quickSortByVacc(CenterReport arr[], int low, int high) {
    if (low >= high) return;
    int pivot = arr[high].totalVaccinations, i = low - 1;
    for (int j = low; j < high; j++)
        if (arr[j].totalVaccinations > pivot) { i++; CenterReport tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp; }
    CenterReport tmp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = tmp;
    int pi = i + 1;
    quickSortByVacc(arr, low, pi - 1);
    quickSortByVacc(arr, pi + 1, high);
}

int binarySearchByID(CenterReport arr[], int n, int targetID) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if      (arr[mid].centerID == targetID) return mid;
        else if (arr[mid].centerID <  targetID) low  = mid + 1;
        else                                    high = mid - 1;
    }
    return -1;
}

// =====================================================================
//  CLEAR SCREEN
// =====================================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// =====================================================================
//  MENU
// =====================================================================
void showMenu() {
    clearScreen();
    cout << CYAN;
    printCentered("==================================================================");
    printCentered("       EPITRACK ENTERPRISE PANDEMIC MANAGEMENT SYSTEM             ");
    printCentered("==================================================================");
    cout << RESET << "\n";

    cout << BLUE << "  ===== HOSPITAL & CENTER SETUP =====\n" << RESET;
    cout << "   1. Register Hospital\n";
    cout << "   2. Hospital & Vaccine Center Management\n";

    cout << BLUE << "  ===== PATIENT MANAGEMENT ===========\n" << RESET;
    cout << "   3. Register Patient\n";
    cout << "   4. Discharge Patient\n";
    cout << "   5. Search / Display Patients\n";

    cout << BLUE << "  ===== EMERGENCY TRIAGE =============\n" << RESET;
    cout << "   6. Emergency Queue Operations MH\n";

    cout << BLUE << "  ===== VACCINATION CENTER ============\n" << RESET;
    cout << "   7. Vaccination Center Queue Operations TQ\n";

    cout << BLUE << "  ===== CONTACT TRACING ===============\n" << RESET;
    cout << "   8. Add Contact Relationship (Undirected)\n";
    cout << "   9. Record Directed Exposure (A exposed B)\n";
    cout << "  10. Graph Traversal & Super Spreaders BD\n";

    cout << BLUE << "  ===== INFECTED VACC QUEUE ===========\n" << RESET;
    cout << "  11. Infected Vaccination Queue Operations TP\n";

    cout << BLUE << "  ===== SUPPLY CHAIN ==================\n" << RESET;
    cout << "  12. Supply Chain Operations TP\n";

    cout << BLUE << "  ===== ANALYTICS =====================\n" << RESET;
    cout << "  13. Vaccination Analytics RQ\n";

    cout << BLUE << "  ===== REPORTING =====================\n" << RESET;
    cout << "  14. Vaccination Analytics Dashboard QB\n";

    cout << BLUE << "  ===== AUDIT =========================\n" << RESET;
    cout << "  15. Audit Log & Undo TS\n";

    cout << RED << "\n   0. Exit\n" << RESET;
    cout << CYAN << "\n==================================================================\n";
    cout << " Enter Choice: " << RESET;
}

// =====================================================================
//  MAIN
// =====================================================================
int main() {
    Controller        ctrl;
    ContactGraph      cGraph;
    InfectedVaccQueue ivq;
    LinkedList        vaccOrderList;

    SupplyChain sc(6);
    sc.setLabel(0, "Manufacturer");
    sc.setLabel(1, "Regional Warehouse");
    sc.setLabel(2, "District Store");
    sc.setLabel(3, "Hospital A");
    sc.setLabel(4, "Hospital B");
    sc.setLabel(5, "Clinic");

    int choice;
    while (true) {
        showMenu();
        cin >> choice; cin.ignore(1000, '\n');
        if (choice == 0) break;
        cout << "\n";

        // Route to the correct module
        if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 14) {
            handle_2307080(choice, ctrl);
        } else if (choice == 6 || choice == 7 || choice == 13 || choice == 15) {
            handle_2307071(choice, ctrl);
        } else if (choice >= 8 && choice <= 12) {
            handle_2307079(choice, ctrl, cGraph, ivq, vaccOrderList, sc);
        } else {
            cout << RED << "  Invalid choice. Please try again.\n" << RESET;
        }

        cout << "\n  Press Enter to continue...";
        cin.get();
    }

    cout << CYAN << "\n  EpiTrack system exited. Stay safe!\n\n" << RESET;
    return 0;
}