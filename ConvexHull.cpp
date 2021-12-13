#include <iostream>
using namespace std;

struct Coordinate
{
    int y;
    int x;
    Coordinate *next;
};

int biggestX = 0;
int biggestY = 0;
int smallestX = 200000000000;
int smallestY = 200000000000;
double xMean;
double yMean;
Coordinate* sortedMergeSort(Coordinate* a, Coordinate* b)
{
    Coordinate* result = NULL;
    //base case
    if (a == NULL)
    {
        return b;
    }
    else if (b == NULL)
    {
        return a;
    }
    else
    {
        if (a->y < b->y)
        {
            result = a;
            result->next = sortedMergeSort(a->next, b);
        }
        else if (a->y > b->y)
        {
            result = b;
            result->next = sortedMergeSort(a, b->next);
        }
        else
        {
            if (a->x <= b->x)
            {
                result = a;
                result->next = sortedMergeSort(a->next, b);
            }
            else if (a->x > b->x)
            {
                result = b;
                result->next = sortedMergeSort(a, b->next);
            }
        }
    }
    return result;
}

void splitter(Coordinate* head, Coordinate** first, Coordinate** second)
{
    Coordinate* fast = head->next;
    Coordinate* slow = head;
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            fast = fast->next;
            slow = slow->next;
        }
    }
    *first = head;
    *second = slow->next;
    slow->next = NULL;
}

void MergeSort(Coordinate** headRef)
{
    Coordinate* head = *headRef;
    Coordinate* first = head;
    Coordinate* second = head;
    if (head == NULL || head->next == NULL)
    {
        return;
    }
    splitter(head, &first, &second);

    MergeSort(&first);
    MergeSort(&second);

    *headRef = sortedMergeSort(first, second);
}


double pseudoCosine(Coordinate center, Coordinate c2)
{
    //this is the quadratic value of cosine
    int adjacent = c2.x - center.x; 
    int opposite = center.y - c2.y;
    int hypotenuse = adjacent * adjacent + opposite * opposite;
    if (adjacent < 0)
    {
        return static_cast<double> (adjacent * adjacent) / (-hypotenuse);
    }
    else
    {
        return static_cast<double> (adjacent * adjacent) / hypotenuse;
    }
}

Coordinate* sortedMergeCos(Coordinate* a, Coordinate* b, Coordinate center)
{
    Coordinate* result = NULL;
    //base case
    if (a == NULL)
    {
        return b;
    }
    else if (b == NULL)
    {
        return a;
    }
    else
    {
        double firstpCosine = pseudoCosine(center, *a);
        double secondpCosine = pseudoCosine(center, *b);
        if (firstpCosine > secondpCosine)
        {
            result = a;
            result->next = sortedMergeCos(a->next, b, center);
        }
        else if (firstpCosine == secondpCosine)
        {
            if (a->y >b->y)
            {
                result = a;
                result->next = sortedMergeCos(a->next, b, center);
            }
            else if (a->y < b->y)
            {
                result = b;
                result->next = sortedMergeCos(a, b->next, center);
            }
            else
            {
                if (a->x >= b->x)
                {
                    result = a;
                    result->next = sortedMergeCos(a->next, b, center);
                }
                else
                {
                    result = b;
                    result->next = sortedMergeCos(a, b->next, center);
                }
            }
        }
        else 
        {
            result = b;
            result->next = sortedMergeCos(a, b->next, center);
        }
    }
    return result;
}

void MergeSortPseudoCosine(Coordinate** headRef, Coordinate center)
{
    Coordinate* head = *headRef;
    Coordinate* first = head;
    Coordinate* second = head;
    if (head == NULL || head->next == NULL)
    {
        return;
    }

    splitter(head, &first, &second);
    MergeSortPseudoCosine(&first, center);
    MergeSortPseudoCosine(&second, center);

    *headRef = sortedMergeCos(first, second, center);
}

void Elimination(Coordinate** headRef, int &pTotal)
{
    //alternately doing elimination until the tail pointer not remain
    //it is regarded that at minimal three nodes exist
    Coordinate* head = *headRef;
    Coordinate* Cur1 = *headRef;
    Coordinate* Cur2 = Cur1->next;
    Coordinate* Cur3 = Cur2->next;

    while (Cur3 != NULL)
    {
        int x1 = Cur2->x - Cur1->x;
        int x2 = Cur3->x - Cur1->x;
        int y1 = Cur2->y - Cur1->y;
        int y2 = Cur3->y - Cur1->y;
        int crossProduct = x1 * y2 - x2 * y1;
        if (crossProduct >= 0)
        {
            //the angle less than 180 degree
            Cur1 = Cur1->next;
            Cur2 = Cur2->next;
            Cur3 = Cur3->next;
        }
        else
        {
            pTotal--;
            //the angle is more than 180 degree
            //chain elimination
            Coordinate* erase = Cur2;
            Cur2 = Cur1;
            Cur2->next = Cur3;
            Coordinate *parseNext = head->next;
            Coordinate* parsePrev = head;
            while (parseNext != NULL && parseNext != Cur1)
            {
                parsePrev = parsePrev->next;
                parseNext = parseNext->next;
            }
            Cur1 = parsePrev;
            delete erase;
        }
    }
}

void sortHead(Coordinate** nonCenterHeadRef, Coordinate center)
{
    int same = 0;
    Coordinate* nonCenterHead = *nonCenterHeadRef;
    Coordinate* current = nonCenterHead->next;
    while (current != NULL)
    {
        if (pseudoCosine(center, *nonCenterHead) == pseudoCosine(center, *current))
        {
            //cout << "Entering!\n";
            same++;
            current = current->next;
        }
        else
        {
            break;
        }
    }

    current = nonCenterHead;
    Coordinate* current2 = nonCenterHead->next;
    for (int i = 0; i < same; i++)
    {
        for (int j = i; j < same - i; j++)
        {
            if (current->y > current2->y || (current->y == current2->y && current->x > current2->x))
            {
                //exchange the current and current2 in this case
                if (current == *nonCenterHeadRef)
                {
                    *nonCenterHeadRef = current2;
                }
                Coordinate* temp = current2->next;
                current2->next = current;
                current->next = temp;
                current2 = temp;
            }
        }
    }
}
int main()
{
    int pTotal;
    cin >> pTotal;
    Coordinate* head = new Coordinate;
    Coordinate* current = head;
    cin >> head->x;
    cin >> head->y;
    Coordinate* tail = head;
    for (int i = 0; i < pTotal-1; i++)
    {
        Coordinate *newPoint = new Coordinate;
        current->next = newPoint;
        current = current->next;
        cin >> current->x;
        cin >> current->y;
        current->next = NULL;
        tail = current;
    }
    current = head;



    //rearrange the link list with merge sort
    MergeSort(&head);

    //printing the final result of the merge sort
    current = head;
    //cout << "====\n";
    /*while (current != NULL)
    {
        cout << current->x << " " << current->y << endl;
        current = current->next;
    }*/
    //cout << "====\n";
    Coordinate* nonCenterHead = current->next;
    MergeSortPseudoCosine(&nonCenterHead, *head);
    current = nonCenterHead;
    /*cout << "Merge cosine result: \n";
    while (current != NULL)
    {
        cout << current->x << " " << current->y << endl;
        current = current->next;
    }
    cout << "===== \n";*/
    sortHead(&nonCenterHead, *head);
    head->next = nonCenterHead;
    //cout << "Editted result:\n";
    /*current = head;
    while (current != NULL)
    {
        cout << current->x << " " << current->y << " " << pseudoCosine(*head, *current) << endl;
        current = current->next;
    }
    */
    if (pTotal < 3)
    {
        current = head;
        while (current != NULL)
        {
            cout << current->x << " " << current->y << endl;
            current = current->next;
        }
        return 0;
    }
    //cout << "Entering!\n";
    //Starting from the head, measure each cross product
    Elimination(&head, pTotal);
    //cout << "Final convex hull result:\n";
    cout << pTotal << endl;
    current = head;
    while (current != NULL)
    {
        cout << current->x << " " << current->y << endl;
        current = current->next;
    }
    return 0;

}
