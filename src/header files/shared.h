#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// ======================= COLOR CODES =======================
extern string RESET;
extern string RED;
extern string GREEN;
extern string YELLOW;
extern string CYAN;
extern string BLUE;

void printCentered(string text, int width = 80);

// =====================================================================
//  TEMPLATE STACK  (Array-based, no STL)
// =====================================================================
template <typename T>
class TemplateStack {
    T   arr[300];
    int topIdx;
public:
    TemplateStack() : topIdx(-1) {}

    bool isEmpty() const { return topIdx == -1; }

    bool push(const T& val) {
        if (topIdx >= 299) return false;
        arr[++topIdx] = val;
        return true;
    }

    T pop() {
        if (topIdx < 0) { T def; return def; }
        return arr[topIdx--];
    }

    T peek() const {
        if (topIdx < 0) { T def; return def; }
        return arr[topIdx];
    }

    int size() const { return topIdx + 1; }

    void display() const {
        if (topIdx == -1) { cout << "  (Stack is empty)\n"; return; }
        for (int i = topIdx; i >= 0; i--)
            cout << "  [" << (topIdx - i + 1) << "] " << arr[i] << "\n";
    }
};

// =====================================================================
//  TEMPLATE QUEUE  (Circular Array, no STL)
// =====================================================================
template <typename T>
class TemplateQueue {
    T   arr[300];
    int frontIdx, rearIdx, cnt;
    static const int CAP = 300;
public:
    TemplateQueue() : frontIdx(0), rearIdx(-1), cnt(0) {}

    bool isEmpty() const { return cnt == 0; }
    int  size()    const { return cnt; }

    bool enqueue(const T& val) {
        if (cnt >= CAP) return false;
        rearIdx = (rearIdx + 1) % CAP;
        arr[rearIdx] = val;
        cnt++;
        return true;
    }

    T dequeue() {
        if (isEmpty()) return T();
        T val = arr[frontIdx];
        frontIdx = (frontIdx + 1) % CAP;
        cnt--;
        return val;
    }

    T front() const {
        if (isEmpty()) return T();
        return arr[frontIdx];
    }

    void display() const {
        if (isEmpty()) { cout << "  (Queue is empty)\n"; return; }
        cout << "\n--- Queue ---\n";
        for (int i = 0; i < cnt; i++) {
            int idx = (frontIdx + i) % CAP;
            cout << "  " << (i + 1) << ". " << arr[idx] << "\n";
        }
    }
};

// =====================================================================
//  SINGLY LINKED LIST  (for Vaccination Order)
// =====================================================================
struct ListNode {
    int       patientID;
    ListNode* next;
    ListNode(int id) : patientID(id), next(NULL) {}
};

class LinkedList {
    ListNode* head;
    int       cnt;
public:
    LinkedList() : head(NULL), cnt(0) {}

    void append(int id) {
        ListNode* node = new ListNode(id);
        if (!head) { head = node; cnt++; return; }
        ListNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
        cnt++;
    }

    void clear() {
        while (head) {
            ListNode* tmp = head;
            head = head->next;
            delete tmp;
        }
        cnt = 0;
    }

    bool isEmpty() const { return head == NULL; }
    int  size()    const { return cnt; }

    int get(int i) const {
        ListNode* cur = head;
        int j = 0;
        while (cur) {
            if (j == i) return cur->patientID;
            cur = cur->next; j++;
        }
        return -1;
    }

    void display() const {
        if (!head) { cout << "  (List is empty)\n"; return; }
        ListNode* cur = head;
        while (cur) {
            cout << "P" << cur->patientID;
            if (cur->next) cout << " -> ";
            cur = cur->next;
        }
        cout << " -> NULL\n";
    }
};

// =====================================================================
//  BST  (Patients)
// =====================================================================
struct PatientNode {
    int        id;
    string     name;
    int        severity;
    PatientNode* left;
    PatientNode* right;
    PatientNode(int i, string n, int s)
        : id(i), name(n), severity(s), left(NULL), right(NULL) {}
};

class BST {
    PatientNode* root;

    PatientNode* insert(PatientNode* node, int id, string name, int severity) {
        if (!node) return new PatientNode(id, name, severity);
        if (id < node->id)       node->left  = insert(node->left,  id, name, severity);
        else if (id > node->id)  node->right = insert(node->right, id, name, severity);
        else cout << RED << "  Patient ID " << id << " already exists.\n" << RESET;
        return node;
    }

    void inorder(PatientNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << "  ID: " << node->id
             << " | Name: " << node->name
             << " | Severity: " << node->severity << "\n";
        inorder(node->right);
    }

    PatientNode* search(PatientNode* node, int id) {
        if (!node || node->id == id) return node;
        if (id < node->id) return search(node->left, id);
        return search(node->right, id);
    }

    PatientNode* findMin(PatientNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    PatientNode* remove(PatientNode* node, int id) {
        if (!node) return NULL;
        if      (id < node->id) node->left  = remove(node->left,  id);
        else if (id > node->id) node->right = remove(node->right, id);
        else {
            if (!node->left)  { PatientNode* t = node->right; delete node; return t; }
            if (!node->right) { PatientNode* t = node->left;  delete node; return t; }
            PatientNode* s = findMin(node->right);
            node->id = s->id; node->name = s->name; node->severity = s->severity;
            node->right = remove(node->right, s->id);
        }
        return node;
    }

public:
    BST() : root(NULL) {}

    void addPatient(int id, string name, int severity) { root = insert(root, id, name, severity); }
    void removePatient(int id)                         { root = remove(root, id); }
    void display()                                     { if (!root) { cout << "  (No patients)\n"; return; } inorder(root); }
    PatientNode* find(int id)                          { return search(root, id); }
};

// =====================================================================
//  MAX HEAP  (Emergency / ICU)
// =====================================================================
struct HeapEntry {
    int    severity;
    string name;
    HeapEntry() : severity(0), name("") {}
    HeapEntry(int s, string n) : severity(s), name(n) {}
};

class MaxHeap {
    HeapEntry arr[200];
    int       sz;

    void heapifyUp(int i) {
        while (i > 1 && arr[i/2].severity < arr[i].severity) {
            HeapEntry tmp = arr[i/2]; arr[i/2] = arr[i]; arr[i] = tmp;
            i /= 2;
        }
    }

    void heapifyDown(int i) {
        while (2*i <= sz) {
            int child = 2*i;
            if (child+1 <= sz && arr[child+1].severity > arr[child].severity) child++;
            if (arr[i].severity < arr[child].severity) {
                HeapEntry tmp = arr[i]; arr[i] = arr[child]; arr[child] = tmp;
                i = child;
            } else break;
        }
    }

public:
    MaxHeap() : sz(0) {}
    bool isEmpty() const { return sz == 0; }

    void insert(int severity, string name) {
        if (sz >= 199) return;
        arr[++sz] = HeapEntry(severity, name);
        heapifyUp(sz);
    }

    HeapEntry extractMax() {
        if (sz == 0) return HeapEntry(-1, "EMPTY");
        HeapEntry top = arr[1];
        arr[1] = arr[sz--];
        heapifyDown(1);
        return top;
    }

    void display() {
        if (sz == 0) { cout << "  (Emergency queue is empty)\n"; return; }
        cout << "\n--- Emergency Priority Queue (Max-Heap) ---\n";
        for (int i = 1; i <= sz; i++)
            cout << "  " << arr[i].name << " | Severity: " << arr[i].severity << "\n";
    }
};

// =====================================================================
//  CONTACT GRAPH  (Undirected + Directed, BFS / DFS / TopoSort)
// =====================================================================
class ContactGraph {
    int adj[200][200];
    int dirAdj[200][200];
    int vertices;

public:
    ContactGraph() : vertices(200) {
        for (int i = 0; i < vertices; i++)
            for (int j = 0; j < vertices; j++)
                adj[i][j] = dirAdj[i][j] = 0;
    }

    bool inRange(int id) const { return id >= 0 && id < vertices; }

    void addContact(int a, int b) {
        if (!inRange(a) || !inRange(b)) return;
        adj[a][b] = adj[b][a] = 1;
    }

    void addExposure(int exposer, int exposed) {
        if (!inRange(exposer) || !inRange(exposed)) return;
        dirAdj[exposer][exposed] = 1;
        adj[exposer][exposed] = adj[exposed][exposer] = 1;
    }

    void BFS(int start) {
        if (!inRange(start)) { cout << "  Invalid patient ID.\n"; return; }
        bool visited[200] = {false};
        // int q[200]; int front = 0, rear = 0;
        // q[rear++] = start;

        TemplateQueue <int> q;
        q.enqueue(start);
        visited[start] = true;

        cout << "\n--- BFS Exposure Trace from Patient " << start << " ---\n  ";
        while (!q.isEmpty()) {
            int cur = q.front();
            q.dequeue();

            cout << "P" << cur << "  ";
            for (int i = 0; i < vertices; i++)
                if (adj[cur][i] && !visited[i]) { visited[i] = true; q.enqueue(i); }
        }
        cout << "\n";
    }

    void DFSUtil(int node, bool visited[]) {
        visited[node] = true;
        cout << "P" << node << "  ";
        for (int i = 0; i < vertices; i++)
            if (adj[node][i] && !visited[i]) DFSUtil(i, visited);
    }

    void DFS(int start) {
        if (!inRange(start)) { cout << "  Invalid patient ID.\n"; return; }
        bool visited[200] = {false};
        cout << "\n--- DFS Infection Cluster from Patient " << start << " ---\n  ";
        DFSUtil(start, visited);
        cout << "\n";
    }

    void identifySuperSpreaders(int threshold = 5) {
        cout << "\n--- Super Spreaders (contacts >= " << threshold << ") ---\n";
        bool found = false;
        for (int i = 0; i < vertices; i++) {
            int cnt = 0;
            for (int j = 0; j < vertices; j++) if (adj[i][j]) cnt++;
            if (cnt >= threshold) { cout << "  Patient P" << i << " -> " << cnt << " contacts\n"; found = true; }
        }
        if (!found) cout << "  No super spreaders found with threshold " << threshold << ".\n";
    }

    bool topoSort(LinkedList& outList, TemplateQueue<int>& outQueue) {
        bool hasNode[200] = {false};
        int  indeg[200]   = {0};

        for (int i = 0; i < vertices; i++)
            for (int j = 0; j < vertices; j++)
                if (dirAdj[i][j]) { indeg[j]++; hasNode[i] = true; hasNode[j] = true; }

        bool anyNode = false;
        for (int i = 0; i < vertices; i++) if (hasNode[i]) { anyNode = true; break; }
        if (!anyNode) {
            cout << "  No directed exposure chains recorded.\n";
            cout << "  Use option 9 to add directed exposures (A exposed B).\n";
            return false;
        }

        outList.clear();

        TemplateQueue<int> q;
        for (int i = 0; i < vertices; i++)
            if (hasNode[i] && indeg[i] == 0) q.enqueue(i);

        int step = 1, processed = 0;
        cout << "\n--- Vaccination Order (Topological Sort on Exposure Chain) ---\n";
        cout << "  Rule: If P_A exposed P_B, vaccinate P_A before P_B.\n\n";

        while (!q.isEmpty()) {
            int u = q.front();
            q.dequeue();
            processed++;

            cout << "  Step " << step++ << ": Vaccinate P" << u;
            bool exposedAny = false;
            for (int v = 0; v < vertices; v++) {
                if (dirAdj[u][v]) {
                    if (!exposedAny) { cout << "  [exposed: P" << v; exposedAny = true; }
                    else cout << ", P" << v;
                    if (--indeg[v] == 0) q.enqueue(v);
                }
            }
            if (exposedAny) cout << "]";
            cout << "\n";

            outList.append(u);
            outQueue.enqueue(u);
        }

        int totalNodes = 0;
        for (int i = 0; i < vertices; i++) if (hasNode[i]) totalNodes++;

        if (processed < totalNodes) {
            cout << RED << "\n  WARNING: Cycle detected in exposure chain!\n";
            for (int i = 0; i < vertices; i++)
                if (hasNode[i] && indeg[i] > 0) cout << "    P" << i << " (unresolved)\n";
            cout << RESET;
            return false;
        }

        cout << GREEN << "\n  All " << totalNodes << " patients ordered successfully.\n" << RESET;
        return true;
    }
};

// =====================================================================
//  SUPPLY CHAIN
// =====================================================================
class SupplyChain {
    int    adj[20][20];
    string labels[20];
    int    V;
public:
    SupplyChain(int v) : V(v) {
        for (int i = 0; i < V; i++) {
            labels[i] = "Node" + to_string(i);
            for (int j = 0; j < V; j++) adj[i][j] = 0;
        }
    }

    void setLabel(int i, string label) { if (i < V) labels[i] = label; }

    void addEdge(int u, int v) {
        if (u >= 0 && v >= 0 && u < V && v < V) adj[u][v] = 1;
    }

    void topoSort() {
        int indeg[20] = {0};
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (adj[i][j]) indeg[j]++;

        TemplateQueue <int> q;
        for (int i = 0; i < V; i++)
            if (indeg[i] == 0) q.enqueue(i);

        cout << "\n--- Supply Chain Distribution Order ---\n";
        int step = 1;
        while (!q.isEmpty()) {
            int u = q.front();
            q.dequeue();
            cout << "  Step " << step++ << ": " << labels[u] << "\n";
            for (int v = 0; v < V; v++)
                if (adj[u][v] && --indeg[v] == 0) q.enqueue(v);
        }
        cout << "\n";
    }
};

// =====================================================================
//  CENTER REPORT  (for analytics)
// =====================================================================
struct CenterReport {
    int centerID;
    int totalVaccinations;
    CenterReport() : centerID(0), totalVaccinations(0) {}
    CenterReport(int id, int total) : centerID(id), totalVaccinations(total) {}
};

void quickSortByID(CenterReport arr[], int low, int high);
void quickSortByVacc(CenterReport arr[], int low, int high);
int  binarySearchByID(CenterReport arr[], int n, int targetID);

// =====================================================================
//  VACCINE CENTER
// =====================================================================
class VaccineCenter {
    int    id;
    string name;
    TemplateQueue<string> vaccQueue;
    TemplateStack<string> audit;
    int    dailyVacc[366];

public:
    VaccineCenter(int i = 0, string n = "") : id(i), name(n) {
        for (int j = 0; j < 366; j++) dailyVacc[j] = 0;
    }

    int    getid()    { return id; }
    string getName()  { return name; }

    void enqueueCitizen(string n) {
        vaccQueue.enqueue(n);
        audit.push("Enqueued: " + n);
        cout << GREEN << "  '" << n << "' added to vaccine center " << name << " queue.\n" << RESET;
    }

    void vaccinateNext(int day) {
        if (day < 1 || day > 365) { cout << RED << "  Invalid day. Must be 1-365.\n" << RESET; return; }
        if (vaccQueue.isEmpty()) { cout << RED << "  Vaccination queue empty.\n" << RESET; return; }
        string c = vaccQueue.dequeue();
         bool writeHeader = false;
        ifstream fin("vaccinated.txt");
        if (!fin.is_open() || fin.peek() == EOF) writeHeader = true;
        fin.close();

        ofstream fout("vaccinated.txt", ios::app);
        if (!fout.is_open()) {
            cout << RED << "  Error opening vaccinated.txt for writing.\n" << RESET;
            return;
        }
        fout.clear();
        if (writeHeader){
            fout << "Citizen\n";
        }
        fout << c << "\n";
        fout.close();
        dailyVacc[day]++;
        audit.push("Vaccinated: " + c + " on day " + to_string(day));
        cout << GREEN << "  Vaccinated: " << c << " (day " << day << ")\n" << RESET;
    }

    void showQueue()   { vaccQueue.display(); }
    void showAudit() {
        cout << "\n--- Audit Logs for " << name << " (most recent first) ---\n";
        audit.display();
    }

    int queryVaccinations(int l, int r) {
        if (l < 1) l = 1;
        if (r > 365) r = 365;
        int sum = 0;
        for (int i = l; i <= r; i++) sum += dailyVacc[i];
        return sum;
    }

    int getTotalVaccinations() {
        int total = 0;
        for (int i = 1; i <= 365; i++) total += dailyVacc[i];
        return total;
    }

    void displayInfo() {
        cout << "  ID: " << id << " | Name: " << name
             << " | Total Vaccinations: " << getTotalVaccinations() << "\n";
    }
};

// =====================================================================
//  HOSPITAL
// =====================================================================
class Hospital {
    int    hospitalID;
    string hospitalName;
    int    totalBeds;
    int    availableBeds;
    int    availableICUBeds;
    BST    patientTree;
    MaxHeap emergencyHeap;

public:
    Hospital() : hospitalID(0), totalBeds(0), availableBeds(0), availableICUBeds(0) {}

    Hospital(int id, string name, int beds, int icu)
        : hospitalID(id), hospitalName(name), totalBeds(beds),
          availableBeds(beds), availableICUBeds(icu) {}

    int    getHospitalID()    { return hospitalID; }
    int    getICUBeds()       { return availableICUBeds; }
    int    getAvailableBeds() { return availableBeds; }
    string getName()          { return hospitalName; }

    void displayInfo() {
        cout << "  ID: " << hospitalID
             << " | Name: " << hospitalName
             << " | Beds: " << availableBeds << "/" << totalBeds
             << " | ICU Beds: " << availableICUBeds << "\n";
    }

    void addPatient(int id, string name, int severity) {
        patientTree.addPatient(id, name, severity);
        if (availableBeds > 0) availableBeds--;
        cout << GREEN << "  Patient '" << name << "' admitted to " << hospitalName << ".\n" << RESET;
    }

    void removePatient(int id) {
        PatientNode* p = patientTree.find(id);
        if (p) {
            patientTree.removePatient(id);
            availableBeds++;
            cout << GREEN << "  Patient " << id << " discharged.\n" << RESET;
        } else {
            cout << RED << "  Patient ID " << id << " not found.\n" << RESET;
        }
    }

    void searchPatient(int id) {
        PatientNode* p = patientTree.find(id);
        if (p) cout << GREEN << "  Found -> ID: " << p->id
                    << " | Name: " << p->name << " | Severity: " << p->severity << "\n" << RESET;
        else   cout << RED   << "  Patient ID " << id << " not found.\n" << RESET;
    }

    void showPatients() { patientTree.display(); }

    bool patientExists(int id) { return patientTree.find(id) != NULL; }

    void addCritical(int severity, string name) {
        emergencyHeap.insert(severity, name);
        cout << YELLOW << "  Critical patient '" << name
             << "' (severity=" << severity << ") added to emergency queue.\n" << RESET;
    }

    void dispatchCritical() {
        HeapEntry e = emergencyHeap.extractMax();
        if (e.severity == -1) { cout << RED << "  Emergency queue is empty.\n" << RESET; return; }
        cout << GREEN << "  Dispatching highest priority: " << e.name
             << " | Severity: " << e.severity << "\n" << RESET;
    }

    void showICU() { emergencyHeap.display(); }
};

// =====================================================================
//  CONTROLLER
// =====================================================================
class Controller {
    Hospital* hospitals[50];
    VaccineCenter* centers[100];
    int            count;
    int            centerCount;
    TemplateStack<string> audit;

public:
    Controller() : count(0), centerCount(0) {}

    void registerHospital(string name, int beds, int icu) {
        if (count >= 50) { cout << RED << "  Hospital limit reached.\n" << RESET; return; }
        hospitals[count] = new Hospital(count + 1, name, beds, icu);
        audit.push("Hospital Registered: " + name);

        bool writeHeader = false;
        ifstream fin("hospitals.txt");
        if (!fin.is_open() || fin.peek() == EOF) writeHeader = true;
        fin.close();

        ofstream fout("hospitals.txt", ios::app);
        if (!fout.is_open()) {
            cout << RED << "  Error opening hospitals.txt for writing.\n" << RESET;
            return;
        }
        fout.clear();
        if (writeHeader){
            fout << "ID,Name,Beds,ICU\n";
        }
        fout << (count+1) << "," << name << "," << beds << "," << icu << "\n";
        fout.close();
        cout << GREEN << "  Hospital '" << name << "' registered with ID=" << (count+1) << ".\n" << RESET;
        
        
        count++;
    }

    void displayAllHospitals() {
        if (count == 0) { cout << "  No hospitals registered.\n"; return; }
        cout << "\n--- Registered Hospitals ---\n";
        for (int i = 0; i < count; i++) hospitals[i]->displayInfo();
    }

    Hospital* getHospital(int id) {
        if (id >= 1 && id <= count) return hospitals[id-1];
        cout << RED << "  Hospital ID " << id << " not found. Valid range: 1-" << count << "\n" << RESET;
        return NULL;
    }

    int getHospitalCount() { return count; }

    void registerCenter(int id, string name) {
        if (centerCount >= 100) { cout << RED << "  Center limit reached.\n" << RESET; return; }
        for (int i = 0; i < centerCount; i++)
            if (centers[i]->getid() == id) { cout << RED << "  Center ID " << id << " already exists.\n" << RESET; return; }
        centers[centerCount++] = new VaccineCenter(id, name);
        audit.push("Vaccine Center Registered: " + name);
         bool writeHeader = false;
        ifstream fin("vaccinecenter.txt");
        if (!fin.is_open() || fin.peek() == EOF) writeHeader = true;
        fin.close();

        ofstream fout("vaccinecenter.txt", ios::app);
        if (!fout.is_open()) {
            cout << RED << "  Error opening vaccinecenter.txt for writing.\n" << RESET;
            return;
        }
        
        if (writeHeader){
            fout << "CenterCount,Name,ID\n";
        }
        fout << centerCount << "," << name << "," << id  << "\n";
        fout.close();
        cout << GREEN << "  Vaccine Center '" << name << "' registered with ID=" << id << ".\n" << RESET;
    }

    void displayAllCenters() {
        if (centerCount == 0) { cout << "  No vaccine centers registered.\n"; return; }
        cout << "\n--- Registered Vaccine Centers ---\n";
        for (int i = 0; i < centerCount; i++) centers[i]->displayInfo();
    }

    VaccineCenter* getcenter(int id) {
        for (int i = 0; i < centerCount; i++)
            if (centers[i]->getid() == id) return centers[i];
        cout << RED << "  Center ID " << id << " not found.\n" << RESET;
        return NULL;
    }

    int            getCenterCount()          { return centerCount; }
    VaccineCenter* getCenterByIndex(int i)   { return centers[i]; }
    int            getCount()                { return count; }

    void showAudit() {
        cout << "\n--- Global Audit Logs (most recent first) ---\n";
        audit.display();
    }

    void undo() {
        if (audit.isEmpty()) { cout << "  Nothing to undo.\n"; return; }
        string last = audit.pop();
        cout << YELLOW << "  Undone: " << last << "\n" << RESET;
    }

    void queryCases(int l, int r) {
        int id;
        cout << "  Enter Vaccine Center ID: "; cin >> id; cin.ignore(1000, '\n');
        VaccineCenter* c = getcenter(id);
        if (c) cout << GREEN << "  Total vaccinations (day " << l << " to " << r << "): "
                    << c->queryVaccinations(l, r) << "\n" << RESET;
    }
};

// =====================================================================
//  INFECTED VACCINATION QUEUE  (VaccQueue2)
// =====================================================================
class InfectedVaccQueue {
    TemplateQueue<int> queue;
    bool               generated;

public:
    InfectedVaccQueue() : generated(false) {}

    bool isGenerated() const { return generated; }

    void buildFromGraph(ContactGraph& cg, LinkedList& ordList) {
        while (!queue.isEmpty()) queue.dequeue();
        generated = false;

        TemplateQueue<int> tempQ;
        bool ok = cg.topoSort(ordList, tempQ);
        if (!ok) { cout << RED << "  VaccQueue2 not built due to cycle.\n" << RESET; return; }

        for (int i = 0; i < ordList.size(); i++)
            queue.enqueue(ordList.get(i));

        generated = true;
        cout << GREEN << "  VaccQueue2 (Infected Vaccination Queue) built with "
             << ordList.size() << " patient(s).\n" << RESET;
    }

    void display() {
        if (!generated) { cout << RED << "  VaccQueue2 not yet generated. Use option 11 sub-option 1.\n" << RESET; return; }
        if (queue.isEmpty()) { cout << "  VaccQueue2 is empty (all patients vaccinated).\n"; return; }
        cout << "\n--- VaccQueue2: Infected Vaccination Queue ---\n";
        queue.display();
    }

    void vaccinateNext(Controller& /*ctrl*/) {
        if (!generated) { cout << RED << "  VaccQueue2 not yet generated. Use option 11 sub-option 1.\n" << RESET; return; }
        if (queue.isEmpty()) { cout << RED << "  VaccQueue2 is empty.\n" << RESET; return; }
        int pid = queue.dequeue();
         bool writeHeader = false;
        ifstream fin("vaccinated.txt");
        if (!fin.is_open() || fin.peek() == EOF) writeHeader = true;
        fin.close();

        ofstream fout("vaccinated.txt", ios::app);
        if (!fout.is_open()) {
            cout << RED << "  Error opening vaccinated.txt for writing.\n" << RESET;
            return;
        }
        fout.clear();
        if (writeHeader){
            fout << "Citizen\n";
        }
        fout << "Patient" << pid << "\n";
        fout.close();
        cout << GREEN << "  [VaccQueue2] Vaccinating infected patient P" << pid << "\n" << RESET;
    }
};