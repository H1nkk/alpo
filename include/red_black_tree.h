#pragma once
#include <optional>

template <typename K, typename V>
class RedBlackTree
{
private:
    struct Node
    {
        enum Color { RED, BLACK };

        Node* pParent;
        Node* pLeft;
        Node* pRight;

        Color color;

        K key;
        V value;

        bool isLeave;

        Node(Node* pParent_, Node* pLeft_, Node* pRight_, Color color_, K key_, V value_, bool isLeave_) :
            pParent(pParent_), pLeft(pLeft_), pRight(pRight_), color(color_), key(key_), value(value_), isLeave(isLeave_)
        {
            if (pLeft != nullptr) pLeft->pParent = this;
            if (pRight != nullptr) pRight->pParent = this;
        }

        static Node* createLeave()
        {
            return new Node(nullptr, nullptr, nullptr, BLACK, {}, {}, true);
        }
    };

    Node* pRoot;
    size_t mSize;

    bool isRightChild(const Node* pNode)
    {
        if (pNode->pParent == nullptr)
        {
            return false;
        }

        return pNode->pParent->pRight == pNode;
    }

    bool isLeftChild(const Node* pNode)
    {
        if (pNode->pParent == nullptr)
        {
            return false;
        }

        return pNode->pParent->pLeft == pNode;
    }

    Node* getGrandparent(const Node* pNode)
    {
        if (pNode != nullptr && pNode->pParent != nullptr)
        {
            return pNode->pParent->pParent;
        }

        return nullptr;
    }

    Node* getUncle(const Node* pNode)
    {
        Node* g = getGrandparent(pNode);
        if (g == nullptr)
        {
            return nullptr;
        }

        if (pNode->pParent == g->pLeft)
        {
            return g->pRight;
        } else
        {
            return g->pLeft;
        }
    }

    void rotateLeft(Node* pNode)
    {
        Node* newp = pNode->pRight;

        newp->pParent = pNode->pParent;
        if (pNode->pParent != nullptr)
        {
            if (pNode == pNode->pParent->pLeft)
            {
                pNode->pParent->pLeft = newp;
            } else
            {
                pNode->pParent->pRight = newp;
            }
        } else
        {
            pRoot = newp;
        }

        pNode->pRight = newp->pLeft;
        if (newp->pLeft != nullptr)
        {
            newp->pLeft->pParent = pNode;
        }

        pNode->pParent = newp;
        newp->pLeft = pNode;
    }

    void rotateRight(Node* pNode)
    {
        Node* newp = pNode->pLeft;

        newp->pParent = pNode->pParent;
        if (pNode->pParent != nullptr)
        {
            if (pNode->pParent->pLeft == pNode)
            {
                pNode->pParent->pLeft = newp;
            } else
            {
                pNode->pParent->pRight = newp;
            }
        } else
        {
            pRoot = newp;
        }

        pNode->pLeft = newp->pRight;
        if (newp->pRight != nullptr)
        {
            newp->pRight->pParent = pNode;
        }

        pNode->pParent = newp;
        newp->pRight = pNode;
    }

    void fixInsert(Node* pNode)
    {
        if (pNode->pParent == nullptr) // pNode корень
        {
            // красим корень в красный
            pNode->color = Node::BLACK;
            return;
        } else if (pNode->pParent->color == Node::BLACK) // дерево сохранило правильность
        {
            return;
        }

        // к этому моменту знаем, что отец красный
        Node* pGrand = getGrandparent(pNode);
        Node* pUncle = getUncle(pNode);

        if (pUncle != nullptr && pUncle->color == Node::RED)
        {
            // дядя красный, можно покрасить его и отца в черный, а деда в красный

            pNode->pParent->color = Node::BLACK;
            pUncle->color = Node::BLACK;
            pGrand->color = Node::RED;

            fixInsert(pGrand); // теперь дед красный и на нем могли сломаться свойства
            return;
        }

        // дядя оказался черным
        // если pNode не левый левый и не правый правый, то повернем дерево
        if (isRightChild(pNode) && isLeftChild(pNode->pParent))
        {
            rotateLeft(pNode->pParent);
            pNode = pNode->pRight;
        } else if (isLeftChild(pNode) && isRightChild(pNode->pParent))
        {
            rotateRight(pNode->pParent);
            pNode = pNode->pLeft;
        }

        pGrand = getGrandparent(pNode);
        pNode->pParent->color = Node::BLACK;
        pGrand->color = Node::RED;
        if (isLeftChild(pNode) && isRightChild(pNode->pLeft))
        {
            rotateRight(pGrand);
        } else
        {
            rotateLeft(pGrand);
        }
    }

public:
    RedBlackTree() : pRoot(nullptr), mSize(0) {}

    size_t size() const noexcept { return mSize; }
    bool empty() const noexcept { return mSize == 0; }

    std::optional<V> find(const K& key) const
    {
        if (pRoot == nullptr)
        {
            return std::nullopt;
        }

        Node* pCur = pRoot;

        while (!pCur->isLeave && pCur->key != key)
        {
            if (pCur->key < key)
            {
                pCur = pCur->pRight;
            }
            else
            {
                pCur = pCur->pLeft;
            }
        }

        if (pCur->isLeave)
        {
            return std::nullopt;
        }
        else
        {
            return pCur->value;
        }
    }

    void insert(const K& key, const V& value)
    {
        if (pRoot == nullptr)
        {
            pRoot = new Node(nullptr, Node::createLeave(), Node::createLeave(), Node::RED, key, value, false);
            fixInsert(pRoot);
            mSize++;
            return;
        }

        Node* pCur = pRoot;

        while (!pCur->isLeave && pCur->key != key)
        {
            if (pCur->key < key)
            {
                pCur = pCur->pRight;
            } else
            {
                pCur = pCur->pLeft;
            }
        }

        if (pCur->isLeave)
        {
            Node* pNew;
            if (isLeftChild(pCur))
            {
                pNew = new Node(pCur->pParent, pCur, Node::createLeave(), Node::RED, key, value, false);
                pNew->pParent->pLeft = pNew;
            }
            else
            {
                pNew = new Node(pCur->pParent, pCur, Node::createLeave(), Node::RED, key, value, false);
                pNew->pParent->pRight = pNew;
            }
            fixInsert(pNew);
            mSize++;
        }
        else
        {
            pCur->value = value;
        }
    }
};