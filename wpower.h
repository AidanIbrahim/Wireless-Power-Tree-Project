// UMBC - CMSC 341 - Spring 2024 - Proj2
#ifndef WPOWER_H
#define WPOWER_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class WirelessPower;
const int MINID = 10000;
const int MAXID = 99999;
const int MINLAT = -90;
const int MAXLAT = 90;
const int MINLONG = -180;
const int MAXLONG = 180;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
enum TREETYPE {BST, AVL, SPLAY};
class Customer{
    public:
    friend class WirelessPower;
    friend class Grader;
    friend class Tester;
    Customer(int id, double lat, double longitude){
        m_id = id; m_latitude = lat; m_longitude = longitude;
        m_left = nullptr; m_right = nullptr; m_height = DEFAULT_HEIGHT;
    }
    int getHeight() const {return m_height;}
    Customer* getLeft() const {return m_left;}
    Customer* getRight() const {return m_right;}
    int getID() const {return m_id;}
    double getLatitude() const {return m_latitude;}
    double getLongitude() const {return m_longitude;}
    void setID(const int id){m_id=id;}
    void setLatitude(double lat) {m_latitude=lat;}
    void setLongitude(double longitude) {m_longitude=longitude;}
    void setHeight(int height){m_height=height;}
    void setLeft(Customer* left){m_left=left;}
    void setRight(Customer* right){m_right=right;}
    string getLatStr() const{
        string text = "";
        int latSeconds = (int)(abs(m_latitude * 3600));
        int latDegrees = latSeconds / 3600;
        latSeconds = latSeconds % 3600;
        int latMinutes = latSeconds / 60;
        latSeconds %= 60;
        char latDirection = (m_latitude >= 0) ? 'N' : 'S';
        text = to_string(latDegrees) + "\u00B0 " + to_string(latMinutes) + "' " + 
                to_string(latSeconds) + "\" " + latDirection;
        return text;
    }
    string getLongStr() const{
        string text = "";
        int longSeconds = (int)(abs(m_longitude * 3600));
        int longDegrees = longSeconds / 3600;
        longSeconds = longSeconds % 3600;
        int longMinutes = longSeconds / 60;
        longSeconds %= 60;
        char longDirection = (m_longitude >= 0) ? 'E' : 'W';
        text = to_string(longDegrees) + "\u00B0 " + to_string(longMinutes) + "' " + 
                to_string(longSeconds) + "\" " + longDirection;
        return text;
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Customer & x );
    private:
    int m_id;
    double m_latitude;
    double m_longitude;
    Customer* m_left;    //the pointer to the left child in the BST
    Customer* m_right;   //the pointer to the right child in the BST
    int m_height;        //the height of node in the BST
};
class WirelessPower{
    public:
    friend class Grader;
    friend class Tester;
    
    WirelessPower(TREETYPE type);
    ~WirelessPower();
    void dumpTree() const;// for debugging purposes
    const WirelessPower & operator=(const WirelessPower & rhs);
    void clear();
    TREETYPE getType() const;
    void insert(const Customer& customer);//inserts into BST, AVL, or SPLAY
    void remove(int id);//only removes from AVL and BST, not from SPLAY
    //changing type from BST or SPLAY to AVL should transfer all nodes to an AVL tree
    void setType(TREETYPE type);
    Customer* m_root;
    private:
    //Customer* m_root;       //the root of the BST
    TREETYPE m_type;        // the type of tree, BST, AVL or SPLAY
    //helper for recursive traversal
    void dump(Customer* customer) const;
    // ***************************************************

    //This function will clear nodes from the tree using a post order traversal
    void postOrderClear(Customer* currNode);

    //This helper funtion returns the height of the tallest child
    int getMaxBranch(Customer* node);

    //This funtion adds inserts to a BST, it does not check balance;
    Customer* insertBST(const Customer& insertNode, Customer* root);

    //This funtion is similar to insertBST, however it checks the balance of nodes once inserted.
    Customer* insertAVL(const Customer& insertNode, Customer* root);

    //This funtion splays the given node to make it the root
    Customer* splay(const int& id, Customer* root);

    /*
    This funtion facilitates removal of nodes, it uses recursion to find the nodes and 
    updates the heights of the nodes accordingly. Does not check balance.
    */
    Customer* removeBST(const int &id, Customer* root);

    //Same as before but checks the balance
    Customer* removeAVL(const int& id, Customer* root);

    //Finds successor, copies data then deletes. Updates heights after and checks balance
    Customer* findSuccessorAVL(Customer* root, Customer*& toCopy);
    
    //Finds successor, copies data then deletes. Updates heights after
    Customer* findSuccessorBST(Customer* root, Customer*& toCopy);

    //This funtion simply checks the balance factor of a node and returns an integer value
    int checkBalance(Customer* node);

    Customer* nodeBalance(Customer* root);

    //Performs a right rotation and returns the parent node after rotation
    Customer* rightRotation(Customer* root);

    //Performs a left rotation and returns the parent node after rotation
    Customer* leftRotation(Customer* root);

    void preOrderCopy(const Customer* rhsRoot);

    //This iterates through postorder to take nodes from a tree and transfer them to an AVL tree without reallocation
    void postOrderTransfer(Customer*& currNode, Customer*& newRoot);

    //This funtion takes a node by reference as an input, and uses AVL insertion logic to place the Nodes in and balance
    Customer* constructAVL(Customer*& transferNode, Customer* root);

    // ***************************************************
};
#endif