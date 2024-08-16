// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"
#include <cmath>
WirelessPower::WirelessPower(TREETYPE type){
    m_type = type;
    m_root = nullptr;
}

WirelessPower::~WirelessPower(){
    clear();
}

void WirelessPower::clear(){  
    postOrderClear(m_root); //Will delete stuff recursively
    m_root = nullptr;
}

void WirelessPower::postOrderClear(Customer* currNode){
    
    if (currNode == nullptr){
        return;
    } 
    postOrderClear(currNode->m_left);
    postOrderClear(currNode->m_right); 
    delete currNode;
    return;
}

void WirelessPower::insert(const Customer& customer){
    int customerKey = customer.getID(); //ID of node we want to insert
    if (customerKey >= MINID && customerKey <= MAXID){ //Makes sure the ID is a valid ID
        if (m_root == nullptr){
            m_root = new Customer(customer.getID(), customer.getLatitude(), customer.getLongitude());
            return;
        }
            switch (m_type){ //cases for each tree
                case BST:
                    m_root = insertBST(customer, m_root);
                    break;
                case AVL:
                    m_root = insertAVL(customer, m_root);
                    break;
                case SPLAY:
                    insertBST(customer, m_root);
                    m_root = splay(customerKey, m_root);
                    break;
            }
    }
}

Customer* WirelessPower::insertBST(const Customer& insertNode, Customer* root){
    int id = insertNode.getID();
    if (root == nullptr){ //The location for the new node has been found
        root = new Customer(insertNode.getID(), insertNode.getLatitude(), insertNode.getLongitude());
        return root;
    }

    if (root->getID() > id){ //Going left
        root->setLeft(insertBST(insertNode, root->getLeft()));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        return root;
    } else if (root->getID() < id){ //Going right
        root->setRight(insertBST(insertNode, root->getRight()));
        root->setHeight(getMaxBranch(root) + 1); 
        return root;
    } else {
        return root; //Node found in tree, do nothing
    }
}

Customer* WirelessPower::insertAVL(const Customer& insertNode, Customer* root){
    int id = insertNode.getID();
    if (root == nullptr){ //The location for the new node has been found
        root = new Customer(insertNode.getID(), insertNode.getLatitude(), insertNode.getLongitude());
        return root;
    }
    if (root->getID() > id){ //Going left
        root->setLeft(insertAVL(insertNode, root->getLeft()));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        root = nodeBalance(root);
        return root;
    } else if (root->getID() < id){ //Going right
        root->setRight(insertAVL(insertNode, root->getRight())); //same as before
        root->setHeight(getMaxBranch(root) + 1); 
        root = nodeBalance(root);
        return root;
    } else {
        return root; //Do nothing
    }
}

Customer* WirelessPower::splay(const int& id, Customer* root) {
    if (root == nullptr || root->getID() == id) { // Base case: Null reached or node found.
        return root;
    }
    
    if (root->getID() > id) { // Need to go left
        if (root->getLeft() == nullptr) {
            return root;
        }
        if (root->getLeft()->getID() > id) { // ZigZig case
            root->getLeft()->setLeft(splay(id, root->getLeft()->getLeft()));
            root = rightRotation(root);
        } else if (root->getLeft()->getID() < id) { // ZigZag case
            root->getLeft()->setRight(splay(id, root->getLeft()->getRight()));
            if (root->getLeft()->getRight() != nullptr) {
                root->setLeft(leftRotation(root->getLeft()));
            }
        }
        
        if (root->getLeft() == nullptr) { //Make sure left isn't null before rotation, root is different because we rotated it earlier
            return root;
        } else {
            return rightRotation(root);
        }

    } else { // Need to go right
        if (root->getRight() == nullptr) {
            return root;
        }

        if (root->getRight()->getID() > id) { // ZagZig case
            root->getRight()->setLeft(splay(id, root->getRight()->getLeft()));
            if (root->getRight()->getLeft() != nullptr) {
                root->setRight(rightRotation(root->getRight()));
            }

        } else if (root->getRight()->getID() < id) { // ZagZag case
            root->getRight()->setRight(splay(id, root->getRight()->getRight()));
            root = leftRotation(root);
        }
        
        if (root->getRight() == nullptr) { //Same as before but for other side
            return root;
        } else {
            return leftRotation(root);
        }
    }
}

Customer* WirelessPower::nodeBalance(Customer* root){
    if (root == nullptr){
        return root;
    }

    int rootBalance = checkBalance(root); //balance factor of node
    int childBalance = 0; //Initalize this here, could be right or left as of right now

    //Left heavy Case
    if (rootBalance > 1){
        childBalance = checkBalance(root->getLeft()); //Check balance of left child to see if we need single or double rotation
        if (childBalance >= 0){ //Left rotation case
            return rightRotation(root);
        } else { //Left Right Case
            root->setLeft(leftRotation(root->getLeft())); //Double rotation
            return rightRotation(root);
        }

    //Right Heavy Case
    } else if (rootBalance < -1){
        childBalance = checkBalance(root->getRight());
        if (childBalance <= 0){ //Single rotation
            return leftRotation(root);
        } else { //Double rotation
            root->setRight(rightRotation(root->getRight()));
            return leftRotation(root);
        }
    } 
    return root;
    
}

Customer* WirelessPower::rightRotation(Customer* root){
    Customer* leftChild = root->getLeft(); //Nodes involed in rotation
    Customer* leftChildRight = leftChild->getRight();

    root->setLeft(leftChildRight);
    leftChild->setRight(root);

    root->setHeight(getMaxBranch(root) + 1);
    leftChild->setHeight(getMaxBranch(leftChild) + 1);

    return leftChild; //return new eldest node
} 

Customer* WirelessPower::leftRotation(Customer* root){
    Customer* rightChild = root->getRight(); //Same as right rotation
    Customer* rightChildLeft = rightChild->getLeft();

    root->setRight(rightChildLeft);
    rightChild->setLeft(root);

    root->setHeight(getMaxBranch(root) + 1);
    rightChild->setHeight(getMaxBranch(rightChild) + 1);

    return rightChild;
}

int WirelessPower::getMaxBranch(Customer* node){

    if (node == nullptr){
        return -1;
    }

    int leftHeight = -1; //Save a step, -1 is the height of a null branch
    int rightHeight = -1;
    if (node->getLeft() != nullptr){//There is a child of the left
        leftHeight = node->getLeft()->getHeight();
    }

    if (node->getRight() != nullptr){
        rightHeight = node->getRight()->getHeight();
    }

    if (leftHeight < rightHeight){ //Returns the greatest height between the siblings
        return rightHeight;
    } else {
        return leftHeight;
    }
}

Customer* WirelessPower::removeBST(const int &id, Customer* root){
    
    //Recursive base case
    if (root == nullptr){ //Node has not been found
        return root;
    }

    if (root->getID() > id){ //Going left
        root->setLeft(removeBST(id, root->getLeft()));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        return root;
    } else if (root->getID() < id){ //Going right
        root->setRight(removeBST(id, root->getRight()));
        root->setHeight(getMaxBranch(root) + 1); 
        return root;
    }
    //My lost child is now found. i.e we found the matching node
    if (root->getLeft() == nullptr){ //This handles the left is null case, and the leaf case
        Customer* tempNode = root->getRight(); //If this is a leaf, this will be null
        delete root;
        return tempNode;
    } else if (root->getRight() == nullptr){ //Right is null but left is not case
        Customer* tempNode = root->getLeft();
        delete root;
        return tempNode;
    } else {  //Two child case
        root->setRight(findSuccessorBST(root->getRight(), root)); //Call a function to manage two child case
        root->setHeight(getMaxBranch(root) + 1);
        return root;
    }
}

void WirelessPower::remove(int id){
   switch (m_type){ //cases for each tree, switch based on type
        case BST:
            m_root = removeBST(id, m_root);
            break;
        case AVL:
            m_root = removeAVL(id, m_root);
            break;
        case SPLAY:
            //Do nothing
            break;
    }
}

Customer* WirelessPower::removeAVL(const int& id, Customer* root){

    if (root == nullptr){ //Node not found
        return root;
    }

    if (root->getID() > id){ //Going left
        root->setLeft(removeAVL(id, root->getLeft()));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        root = nodeBalance(root); //Balance nodes on way back up
        return root;
    } else if (root->getID() < id){ //Going right
        root->setRight(removeAVL(id, root->getRight()));
        root->setHeight(getMaxBranch(root) + 1); 
        root = nodeBalance(root); //Balance nodes
        return root;
    } //The Node is found
    if (root->getLeft() == nullptr){ //This handles the left is null case, and the leaf case
        Customer* tempNode = root->getRight(); //If this is a leaf, this will be null
        delete root;
        return tempNode;
    } else if (root->getRight() == nullptr){ //Right is null but left is not case
        Customer* tempNode = root->getLeft();
        delete root;
        return tempNode;
    } else {  //Two child case
        root->setRight(findSuccessorAVL(root->getRight(), root));
        root->setHeight(getMaxBranch(root) + 1);
        root = nodeBalance(root); //Balance nodes
        return root;
    }
}

Customer* WirelessPower::findSuccessorAVL(Customer* root, Customer*& toCopy){
     if (root->getLeft() == nullptr){ //This is the leftmost node
        Customer *tempNode = root->getRight();

        toCopy->setID(root->getID()); //Copy data over
        toCopy->setLatitude(root->getLatitude());
        toCopy->setLongitude(root->getLongitude());\

        delete root; //Deallocate and return
        return tempNode;
    } else {
        int balanceFactor = 0;
        root->setLeft(findSuccessorAVL(root->getLeft(), toCopy));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        balanceFactor = checkBalance(root); //Rebalance all nodes on way back up
        if (balanceFactor > 1 || balanceFactor < -1){
            root = nodeBalance(root);
        }
        return root;
    }
}

Customer* WirelessPower::findSuccessorBST(Customer* root, Customer*& toCopy){
    if (root->getLeft() == nullptr){ //This is the leftmost node
        Customer *tempNode = root->getRight();

        toCopy->setID(root->getID()); //Copy data over
        toCopy->setLatitude(root->getLatitude());
        toCopy->setLongitude(root->getLongitude());

        delete root;
        return tempNode;
    }
        root->setLeft(findSuccessorBST(root->getLeft(), toCopy));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        return root;
}

TREETYPE WirelessPower::getType() const{
    return m_type;
}

void WirelessPower::setType(TREETYPE type){
    if (m_type != AVL && type == AVL){
        Customer* newRoot = nullptr;
        postOrderTransfer(m_root, newRoot);
        m_root = newRoot;
    }
    m_type = type;
}

int WirelessPower::checkBalance(Customer* node){
    if (node == nullptr){ //No children no balance factor
        return 0;
    }
    int leftHeight = -1; //Set to default, nullptr is a -1 height in my implementation
    int rightHeight = -1;
    if (node->getLeft() != nullptr){ //Get child heights
        leftHeight = node->getLeft()->getHeight();
    }

    if (node->getRight() != nullptr){
        rightHeight = node->getRight()->getHeight();
    }

    return leftHeight - rightHeight;
}

const WirelessPower & WirelessPower::operator=(const WirelessPower & rhs){
    if (&rhs.m_root == &m_root){ //Self assignment check
        return *this;
    }
    clear();
    if (rhs.m_root == nullptr){
        m_type = rhs.m_type;
        m_root = nullptr;
        return *this;
    }
    m_root = nullptr;
    m_type = BST; //This prevents rebalancing during construction while copying an AVL or SPLAY tree, stupid fix but it works
    preOrderCopy(rhs.m_root);
    m_type = rhs.m_type;
    return *this;
}

void WirelessPower::preOrderCopy(const Customer* rhsRoot){
    if (rhsRoot == nullptr){ //This generates a copy by going preorder. By inserting nodes in preorder the BST will sort into the exact same tree
        return;
    }
    Customer copyNode(rhsRoot->getID(), rhsRoot->getLatitude(), rhsRoot->getLongitude());
    insert(copyNode);
    preOrderCopy(rhsRoot->getLeft());
    preOrderCopy(rhsRoot->getRight());
}

void WirelessPower::postOrderTransfer(Customer*& currNode, Customer*& newRoot){
    if (currNode == nullptr){ //Recursive bass casse
        return;
    }

    Customer* nextNode = currNode->getLeft(); //Go left
    postOrderTransfer(nextNode, newRoot); 

    nextNode = currNode->getRight();
    postOrderTransfer(nextNode, newRoot); //Go right

    newRoot = constructAVL(currNode, newRoot);

    currNode = nullptr; //Node becomes nullptr
    return;
}

Customer* WirelessPower::constructAVL(Customer*& transferNode, Customer* root){
    int id = transferNode->getID(); //Basically an insert function with but instead of allocating new memory it places already allocated nodes in a AVL balanced way.
    if (root == nullptr){ //The location for the new node has been found
        transferNode->setRight(nullptr);
        transferNode->setLeft(nullptr);
        transferNode->setHeight(0);
        return transferNode;

    }
    if (root->getID() > id){ //Going left
        root->setLeft(constructAVL(transferNode, root->getLeft()));
        root->setHeight(getMaxBranch(root) + 1); //update the heights of the nodes on the way back up
        root = nodeBalance(root);
        return root;
    } else if (root->getID() < id){ //Going right
        root->setRight(constructAVL(transferNode, root->getRight()));
        root->setHeight(getMaxBranch(root) + 1); 
        root = nodeBalance(root);
        return root;
    } else {
        return root;
    }
}

void WirelessPower::dumpTree() const {dump(m_root);}
void WirelessPower::dump(Customer* customer) const{
    if (customer != nullptr){
        cout << "(";
        dump(customer->m_left); //first visit the left child
        cout << customer->m_id << ":" << customer->m_height;//second visit the node itself
        dump(customer->m_right);//third visit the right child
        cout << ")";
    }
}
ostream& operator<<(ostream& sout, const Customer & x ) {
    sout << x.m_id << " (" << x.getLatStr() << ", " << x.getLongStr() << ")" << endl;;
  return sout;
}
