#include <stdio.h>
#include <math.h>

#define nod_size(nod) (nod.t - nod.f + 1)

struct nod
{
    int v, t, f, pv;
    int isPending = 0;
};

class SegmentTree
{
    nod *heap;
    int *array;
    int size;
    int qrv;

public:
    SegmentTree(int *arr, int len, int ret_val)
    {
        array = arr;
        size = (int)(8 * (1 << (int)(log2(len) + 1)));
        heap = (nod *)malloc(size * sizeof(nod));
        build(1, 0, len);
        qrv = ret_val;
    }

public:
    void update(int from, int to, int value)
    {
        update(1, from, to, value);
    }
    int query(int from, int to)
    {
        return query(1, from, to);
    }

public:
    inline int function(int left, int right);
    inline int change_val(nod n, int update_val);

private:
    void build(int i, int from, int size)
    {
        heap[i].f = from;
        heap[i].t = from + size - 1;
        heap[i].isPending = false;
        if (size == 1)
        {
            heap[i].v = array[from];
        }
        else
        {
            build(2 * i, from, size / 2);
            build(2 * i + 1, from + size / 2, size - size / 2);
            heap[i].v = function(heap[2 * i].v, heap[2 * i + 1].v);
        }
    }
    int query(int i, int from, int to)
    {
        nod &n = heap[i];

        if (n.isPending == true && contains(n.f, n.t, from, to))
        {
            return change_val(n, n.pv);
        }

        if (contains(from, to, n.f, n.t))
        {
            return heap[i].v;
        }

        if (intersects(from, to, n.f, n.t))
        {
            propagate(i);
            int left = query(2 * i, from, to);
            int right = query(2 * i + 1, from, to);
            return function(left, right);
        }
        
        return qrv;
    }

    void update(int v, int from, int to, int value)
    {
        nod &n = heap[v];
        if (contains(from, to, n.f, n.t))
        {
            change(&n, value);
        }

        if (nod_size(n) == 1)
            return;

        if (intersects(from, to, n.f, n.t))
        {
            propagate(v);

            update(2 * v, from, to, value);
            update(2 * v + 1, from, to, value);

            n.v = function(heap[2 * v].v, heap[2 * v + 1].v);
        }
    }

    void propagate(int v)
    {
        nod n = heap[v];

        if (n.isPending == 1)
        {
            change(&heap[2 * v], n.pv, 1);
            change(&heap[2 * v + 1], n.pv, 1);
            n.isPending = 0;
        }
    }

    void change(nod *n, int value, int pending = 0)
    {
        n->pv = value;
        n->isPending = pending;
        n->v = change_val(*n, value);
        array[n->f] = value;
    }

    inline bool contains(int from1, int to1, int from2, int to2)
    {
        return from2 >= from1 && to2 <= to1;
    }

    inline bool intersects(int from1, int to1, int from2, int to2)
    {
        return from1 <= from2 && to1 >= from2     //  (.[..)..] or (.[...]..)
               || from1 >= from2 && from1 <= to2; // [.(..]..) or [..(..)..
    }
};

int SegmentTree::function(int left, int right)
{
    return __min(left, right);
}

int SegmentTree::change_val(nod n, int v)
{
    int e = nod_size(n) * v;
    return n.v + e;
}

int main()
{
    int arr[] = {1, 3, 4, 2, 1, -2, 4};
    SegmentTree seg(arr, 7, INT_MAX);
    printf("%d\n", seg.query(0, 2));
    seg.update(0, 2, 1);
    printf("%d\n", seg.query(0, 2));
    seg.update(0, 2, 4);
    printf("%d\n", seg.query(0, 2));
}
