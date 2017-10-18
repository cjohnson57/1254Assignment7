/*
        Author: W. Douglas Lafield
        OrderedList.h
*/
#ifndef _NODE_GUARD
#define _NODE_GUARD 1

using namespace std;

template <class T>
class Node
{
	public:
		T *value;
		Node *left;
		Node *right;
		Node *parent;
		char parentsLorR;

		Node(T *newValue, Node * par, char side) { value = newValue; parent = par; parentsLorR = side; left = right = nullptr; }
		~Node()
		{
			delete left;
			delete right;
			delete value;
		}
		
		/* this function is optional */
		Node *searchReturnParent(T *target, int (*defaultCompare)(T, T));
		
		bool insert(T *newItem, int (*defaultCompare)(T, T));
		/* If new item exists in tree, return false
		 * otherwise, add item to tree, and return true */
		 
		bool remove(T *target, int (*defaultCompare)(T, T));
		/* If target item exists in tree, remove it, and return true
		 * otherwise, return false */
		 
		T *search(T *target, int (*defaultCompare)(T, T));
		/* If target exists in tree, return pointer to the target
		 * otherwise, return null
		 * */
		 
		void traverse(void (*visit)(T));
		/* For each node, run the visit function */
		
		double traverseDouble(double initialvalue, double (*visit)(double, T));
		/* for each node, run the visit function 
		 * The initial value is used as an arguement for the first 
		 * visit.  The return value for each visit is used as an 
		 * arguement for the next visit.  The return value for the 
		 * final visit is returned.
		 * */
		 
		int traverseInt(int initialValue, int (*visit)(int, T));
		/* for each node, run the visit function 
		 * The initial value is used as an arguement for the first 
		 * visit.  The return value for each visit is used as an 
		 * arguement for the next visit.  The return value for the 
		 * final visit is returned.
		 * */
		 
		void traverseOut(ofstream *f, void (*visit)(ofstream *, T));
		/* The file *f should already be open prior to calling traverseOut.
		 * 
		 * For each node, run the visit function
		 * */
};

/***************************************************/

/* this function is optional */
template <class T>
Node<T> *Node<T>::searchReturnParent(T *target, int (*defaultCompare)(T, T))
{
} /* search return parent */

/***************************************************/

template <class T>
T *Node<T>::search(T *target, int (*defaultCompare)(T, T))
{
	if (this == nullptr) //target does not exist
	{
		return nullptr;
	}
	int compare = defaultCompare(*target, *value);
	if (compare == 0) //target is equal to value
	{
		return value;
	}
	else if (compare == -1) //target is less than current node
	{
		return left->search(target, defaultCompare);
	}
	else if (compare == 1) //target is greater than current node
	{
		return right->search(target, defaultCompare);
	}
} /* search */

/***************************************************/

template <class T>
bool Node<T>::insert(T *newItem, int (*defaultCompare)(T, T))
{
	int compare = defaultCompare(*newItem, *value);
	if (compare == 0) //target is equal to value
	{
		return false;
	}
	else if (compare == -1) //target is less than current node
	{
		if (left == nullptr)
		{
			Node * newNode = new Node(newItem, this, 'L');
			left = newNode;
			return true;
		}
		else
		{
			return left->insert(newItem, defaultCompare);
		}
	}
	else if (compare == 1) //target is greater than current node
	{
		if (right == nullptr)
		{
			Node * newNode = new Node(newItem, this, 'R');
			right = newNode;
			return true;
		}
		else
		{
			return right->insert(newItem, defaultCompare);
		}
	}
} /* insert */

/***************************************************/

template <class T>
bool Node<T>::remove(T *target, int (*defaultCompare)(T, T))
{
	if (this == nullptr) //target does not exist
	{
		return false;
	}
	int compare = defaultCompare(*target, *value);
	if (compare == 0) //target is equal to value
	{
		if (left == nullptr && right == nullptr) //If current node is a leaf
		{
			if (parentsLorR == 'L') //Is the parent's left child
			{
				parent->left = nullptr;
				return true;
			}
			else if (parentsLorR == 'R') //Is the parent's right child
			{
				parent->right = nullptr;
				return true;
			}
			else if (parentsLorR == 'N') //Is the root node
			{
				delete this;
				return true;
			}
		}
		else if (left != nullptr && right == nullptr) //Has left node but no right node
		{
			if (parentsLorR == 'L') //Is the parent's left child
			{
				parent->left = left;
				left->parent = parent;
				return true;
			}
			else if (parentsLorR == 'R') //Is the parent's right child
			{
				parent->right = left;
				left->parent = parent;
				return true;
			}
			else if (parentsLorR == 'N') //Is the root node, in this situation there is only a root node and its left node
			{
				T *temp = left->value;
				left->value = value;
				value = temp;
				return left->remove(target, defaultCompare);
			}
		}
		else if (left == nullptr && right != nullptr) //Has right node but no left node
		{
			if (parentsLorR == 'L') //Is the parent's left child
			{
				parent->left = right;
				right->parent = parent;
				return true;
			}
			else if (parentsLorR == 'R') //Is the parent's right child
			{
				parent->right = right;
				right->parent = parent;
				return true;
			}
			else if (parentsLorR == 'N') //Is the root node, in this situation there is only a root node and its right node
			{
				T *temp = right->value;
				right->value = value;
				value = temp;
				return right->remove(target, defaultCompare);
			}
		}
		else //Node has left node and right node
		{
			//Using predecessor
			if (left->right == nullptr) //If the left child has no right child
			{
				T *temp = left->value;
				left->value = value;
				value = temp;
			}
			else
			{
				Node * predecessor = left->right;
				while (predecessor->right != nullptr)
				{
					predecessor = predecessor->right;
				}
				T *temp = predecessor->value;
				predecessor->value = value;
				value = temp;
			}
			return left->remove(target, defaultCompare);
		}
	}
	else if (compare == -1) //target is less than current node
	{
		return left->remove(target, defaultCompare);
	}
	else if (compare == 1) //target is greater than current node
	{
		return right->remove(target, defaultCompare);
	}
} /* remove */

/***************************************************/

template <class T>
void Node<T>::traverse(void (*visit)(T))
{
	if (left != nullptr)
	{
		left->traverse(visit);
	}
	visit(*value);
	if (right != nullptr)
	{
		right->traverse(visit);
	}
} /* traverse */

/***************************************************/

template <class T>
double Node<T>::traverseDouble(double initialValue, double (*visit)(double, T))
{
	if (left != nullptr)
	{
		initialValue = left->traverseDouble(initialValue, visit);
	}
	initialValue = visit(initialValue, *value);
	if (right != nullptr)
	{
		initialValue = right->traverseDouble(initialValue, visit);
	}
	return initialValue;
} /* traverse double */

/***************************************************/

template <class T>
int Node<T>::traverseInt(int initialValue, int (*visit)(int, T))
{
	if (left != nullptr)
	{
		initialValue = left->traverseInt(initialValue, visit);
	}
	initialValue = visit(initialValue, *value);
	if (right != nullptr)
	{
		initialValue = right->traverseInt(initialValue, visit);
	}
	return initialValue;
} /* traverse int */

/***************************************************/

template <class T>
void Node<T>::traverseOut(ofstream *f, void (*visit)(ofstream *, T))
{
	if (left != nullptr)
	{
		left->traverseOut(f, visit);
	}
	visit(f, *value);
	if (right != nullptr)
	{
		right->traverseOut(f, visit);
	}
} /* traverse out */

/***************************************************/

#endif
