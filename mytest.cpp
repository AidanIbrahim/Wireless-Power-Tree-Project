// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>


const char *PASS = "     TEST PASSED: ";
const char *FAIL = "#####TEST FAILED: "; 
const int TEST_ELEMENTS = 10000;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
    public:


        /*Case: any tree
        Expectation: this funtion simply calls checkIfSorted. I did NOT want 
        cout in the loops, this saves a headache
        */
        bool bstPropertyTest(const WirelessPower& tree){
            if (checkIfSorted(tree.m_root)){
                return true;
            } else {
                cout << FAIL << "The tree violates the sorted property" << endl;
                return false;
            }
        }

        /*Case: any tree
        Expectation: this funtion makes sure all nodes follow the sorted property
        i.e lower to the left, higher to the right
        */
        bool checkIfSorted(Customer* root, int prevID = 0){
            if (root == nullptr){
                return true;
            }

            if (!checkIfSorted(root->getLeft(), prevID)){
                return false;
            }

            if (root->getID() < prevID){
                return false;
            }

            prevID = root->getID();

            if (!checkIfSorted(root->getRight(), prevID)){
                return false;
            }
            
            return true;
        }

        /*
        Funtion: =operator
        Case: =operator is used on a pair of trees
        Expectation: this funtion calls the test funtion below, I just made this one to avoid cout statements in a loop.
        Also the types should be the same
        */
        bool assignmentOpCheck(const WirelessPower& treeOne, const WirelessPower& treeTwo){
            if (assignmentOperatorBST(treeOne.m_root, treeTwo.m_root) && treeOne.getType() == treeTwo.getType()){
                if (&treeTwo.m_root != &treeOne.m_root){ //Check if deep copy
                    return true;
                }
                    cout << FAIL << "Not a deep copy" << endl;
                    return false;
            } else {
                cout << FAIL << "The tree was not assigned properly, or the trees are not identical." << endl;
                return false;
            }
        }

        /*
        Funtion: =operator
        Case: =operator is used on a pair of trees
        Expectation: this funtion compares all the nodes in order, as well as their memory addresses.
        the IDs should be the same, but the addresses should be different. Will return false if empty tree is passed in
        */
        bool assignmentOperatorBST(Customer* root, Customer* assignRoot){
            if (root == nullptr && assignRoot == nullptr){
                    return true;
                } else if (root == nullptr || assignRoot == nullptr){ //IF only one is nullptr
                    return false;
                }

            if (!assignmentOperatorBST(root->getLeft(), assignRoot->getLeft())){
                return false;
            }

            if (root->getID() != assignRoot->getID()){
                return false;
            }


            if (!assignmentOperatorBST(root->getRight(), assignRoot->getRight())){
                return false;
            }
            
            return true;
        }

        bool assignmentOperatorEmpty(WirelessPower tree1, WirelessPower tree2){
            if (tree2.m_root != nullptr || tree1.m_root != nullptr){
                return false;
            }
            return true;
         }

        /*
        Funtion: splay
        Case: a node is splayed after an insert using the private helper funtion splay
        Expectation: the root should be the node 
        */
        bool splayRootTest(const WirelessPower& splayTree, const Customer& checkCustomer){
            if (splayTree.m_root->getID() != checkCustomer.getID()){
                return false;
            }
                return true;
            }
        

        //This is just to call the recursive test below. Same deal, no couts in loops.
        bool heightTest(Customer* root){
            if (heightTestRecursive(root)){
                return true;
            } else {
                cout << FAIL << "The heights are not correct for this tree" << endl;
                return false;
            }
        }

        /*
        Funtion: insert or remove on any tree
        Case: A tree exists
        Expectation: This is a recursive funtion to check the heights. Each height should be one greater than 
        the max height between its children, and the heights should be 0 at the leaves
        */
        bool heightTestRecursive(Customer* root){
            if (root == nullptr){
                return true;
            }

                int leftHeight = -1;
                int rightHeight = -1;
                if (root->getLeft() != nullptr){
                    leftHeight = root->getLeft()->getHeight();
                }
                if (root->getRight() != nullptr){
                    rightHeight = root->getRight()->getHeight();     
                }
                if (leftHeight > rightHeight){
                    if (root->getHeight() != leftHeight + 1){
                        return false;
                    }
                } else {
                    if (root->getHeight() != rightHeight + 1){
                        return false;
                    }
                }
                return heightTest(root->getRight()) && heightTest(root->getLeft());

        }


        //This is just to call the recursive test below. Same deal, no couts in loops.
        bool balanceTest(Customer* root){
            if (balanceTestRecursive(root)){
                return true;
            } else {
                cout << FAIL << "The tree is unbalanced" << endl;
                return false;
            }
        }

        /*
        Funtion: insert or remove on any tree
        Case: An AVL tree exists, this can be called to check any AVL tree
        Expectation: This is a recursive funtion to check the balance. the left branch height - the right branch height should never be 
        */
        bool balanceTestRecursive(Customer* root){
            if (root == nullptr){
                return true;
            }

                int leftBranch = 0;
                int rightBranch = 0;
                int balanceFactor = 0;
                if (root->getLeft() != nullptr){
                    leftBranch = root->getLeft()->getHeight();
                }
                if (root->getRight() != nullptr){
                    rightBranch = root->getRight()->getHeight();     
                }

                balanceFactor = leftBranch - rightBranch;

                if (balanceFactor > 1 || balanceFactor < -1){
                    return false;
                }
                return balanceTestRecursive(root->getRight()) && balanceTestRecursive(root->getLeft());
        }

        bool removeTest(vector<int> deleteList, WirelessPower& tree){

            while(!deleteList.empty()){
                tree.remove(deleteList.back());
                deleteList.pop_back();
            }
            if (!heightTest(tree.m_root)) {
                cout << "-----------The last error occured during removal" << endl;
                return false;
            }
            if (tree.m_type == AVL){
                if (!balanceTest(tree.m_root)){
                    cout << "-----------The last error occured during removal" << endl;
                    return false;
                }
            }
            return true;
        }

        //Tests multiple clears on a tree
        bool clearTest(WirelessPower tree){
            tree.insert(Customer(10000, 0, 0));
            tree.clear();
            if (tree.m_root != nullptr){
                return false;
            }
            tree.clear();
            if (tree.m_root != nullptr){
                return false;
            }
                return true;
        }
};
int main(){

    //To change the amount of Nodes allocated in each test, edit TEST_ELEMENTS constant at top of file. Default is 10,000.
    Random idGen(MINID,MAXID); //Generators for nodes
    idGen.setSeed(7);
    Random rndLat(MINLAT, MAXLAT, UNIFORMREAL);
    rndLat.setSeed(5);
    Random rndLong(MINLONG, MAXLONG, UNIFORMREAL);
    rndLong.setSeed(9);
    Tester mainTest;

    WirelessPower tree1(BST); 
    vector<int> deleteIDs; //This will hold a selection of nodes we want to delete
    cout << "####################BST TESTS" <<  endl;
    for (int i = 0; i < TEST_ELEMENTS; i++){
        int newID = idGen.getRandNum();
        tree1.insert(Customer(newID, rndLat.getRealRandNum(), rndLong.getRealRandNum()));
        if ((newID % 2) == 0){ //Collects all even nodes so a large variety will be removed
            deleteIDs.push_back(newID);
        }
    }
    
    WirelessPower tree2(AVL);
    tree2 = tree1; //Test Assignment operator on full tree
    if (mainTest.bstPropertyTest(tree1) && mainTest.assignmentOpCheck(tree1, tree2) && mainTest.heightTest(tree1.m_root) && mainTest.removeTest(deleteIDs, tree1)){
        cout << PASS << "The basic BST has passed all tests" << endl;
    }

    cout << "####################AVL TESTS" <<  endl;
    tree2.clear(); //Test Clear, check for memory leaks after program runs
    tree1.clear();
    tree2.setType(AVL);
    for (int i = 0; i < TEST_ELEMENTS; i++){
        int newID = idGen.getRandNum();
        tree2.insert(Customer(newID, rndLat.getRealRandNum(), rndLong.getRealRandNum()));
        if ((newID % 2) == 0){
            deleteIDs.push_back(newID);
        }
    }

    if (mainTest.bstPropertyTest(tree2) && mainTest.balanceTest(tree2.m_root) && mainTest.heightTest(tree2.m_root) && mainTest.removeTest(deleteIDs, tree2)){
        cout << PASS << "The AVL tree has passed all tests" << endl;
    }

    cout << "####################SPLAY TESTS" <<  endl;
    WirelessPower tree3(SPLAY);
    bool splayRootBool = true;
    for (int i = 0; i < TEST_ELEMENTS; i++){
        int newID = idGen.getRandNum();
        Customer testCustomer = Customer(newID, rndLat.getRealRandNum(), rndLong.getRealRandNum());
        tree3.insert(testCustomer);
        if (!mainTest.splayRootTest(tree3, testCustomer)){ //Test splay property, that m_root is the most recently inserted node.
            splayRootBool = false;
        }
        if ((newID % 2) == 0){
            deleteIDs.push_back(newID);
        }
    } 

    if (!splayRootBool){
            cout << FAIL << "The SPLAY tree did not correctly splay an inserted node" << endl;
    }

    if (mainTest.bstPropertyTest(tree3) && splayRootBool && mainTest.heightTest(tree3.m_root) && mainTest.removeTest(deleteIDs, tree3)){
        cout << PASS << "The SPLAY tree has passed all tests" << endl;
    }

    cout << "####################SET TYPE" <<  endl;
    WirelessPower tree4(BST);
    for (int i = 0; i < TEST_ELEMENTS; i++){
        Customer testCustomer = Customer(idGen.getRandNum(), rndLat.getRealRandNum(), rndLong.getRealRandNum());
        tree4.insert(testCustomer);
    }

    tree4.setType(AVL);

    if (mainTest.bstPropertyTest(tree4) && mainTest.heightTest(tree4.m_root) && mainTest.balanceTest(tree4.m_root) && tree4.getType() == AVL){
        cout << PASS << "Set Type to AVL balances the tree correctly." << endl;
    }

    cout << "####################MISC TESTS" <<  endl;
    cout << "Clearing empty tree" <<  endl;
    if (!mainTest.clearTest(tree1)){
         cout << PASS << "Clear works properly, check for memory leaks" << endl;
    }
    cout << "Assigning an empty tree" <<  endl;
    tree3 = tree1;
    if (mainTest.assignmentOperatorEmpty(tree3, tree1)){
        cout << PASS << "Case passed" << endl;
    }
    tree3.clear();
    tree1.clear();
    cout << "Assigning two empty trees together" <<  endl;
    tree3 = tree1;
    if (mainTest.assignmentOperatorEmpty(tree3, tree1)){
        cout << PASS << "Case passed" << endl;
    }

    cout << "Self assignment test" <<  endl;

    for (int i = 0; i < 10; i++){
        int newID = idGen.getRandNum();
        tree1.insert(Customer(newID, rndLat.getRealRandNum(), rndLong.getRealRandNum()));
    }
    tree3 = tree1; //tree3 is a copy of tree one before it has been self assigned, we can compare it to after to see if anything changed.
    tree1 = tree1;
    if (mainTest.assignmentOpCheck(tree3, tree1)){ //Reusing this test since if the tree has been changed from self assignment this will detect it.
        cout << PASS << "Case passed" << endl;
    }
    cout << "TEST PROGRAM RAN TO COMPLETION" << endl;   
    return 0;
}