// MIT License

// Copyright (c) 2023 Ariyudo Pertama

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma region Segment Tree

#ifndef _SEGMENT_TREE_VARIANT
#define _SEGMENT_TREE_VARIANT 1

namespace segment_tree_variant
{
    template <typename Tp>
    struct sum
    {
        using DatTp = Tp;
        using TrnTp = Tp;
        using RetTp = Tp;

        static DatTp update(RetTp lval, RetTp rval=0)
        {
            return lval + rval;
        }

        static TrnTp get_transition(TrnTp lval, TrnTp rval=0)
        {
            return lval + rval;
        }

        static RetTp get(TrnTp val)
        {
            return val;
        }
    };

    template <typename Tp>
    struct bitwise_xor
    {
        using DatTp = Tp;
        using TrnTp = Tp;
        using RetTp = Tp;

        static DatTp update(RetTp lval, RetTp rval)
        {
            return lval ^ rval;
        }

        static TrnTp get_transition(TrnTp val)
        {
            return val;
        }

        static RetTp get(TrnTp val)
        {
            return val;
        }
    };

    template <typename Tp>
    struct bitwise_and
    {
        using DatTp = Tp;
        using TrnTp = Tp;
        using RetTp = Tp;

        static DatTp update(RetTp lval, RetTp rval)
        {
            return lval & rval;
        }

        static TrnTp get_transition(TrnTp val)
        {
            return val;
        }

        static RetTp get(TrnTp val)
        {
            return val;
        }
    };

    template <typename Tp>
    struct bitwise_or
    {
        using DatTp = Tp;
        using TrnTp = Tp;
        using RetTp = Tp;

        static DatTp update(RetTp lval, RetTp rval)
        {
            return lval | rval;
        }

        static TrnTp get_transition(TrnTp val)
        {
            return val;
        }

        static RetTp get(TrnTp val)
        {
            return val;
        }
    };

    template <typename Tp>
    struct max_subarray_sum
    {
        using DatTp = vector<Tp>;
        using TrnTp = vector<Tp>;
        using RetTp = Tp; 

        static DatTp update(RetTp val)
        {
            return DatTp(4, val);
        }
        static DatTp update(DatTp lval, DatTp rval)
        {
            return DatTp
            {
                max({
                    lval[0],
                    rval[0],
                    lval[1] + rval[2]
                }),
                max(
                    rval[1],
                    lval[1] + rval[3]
                ),
                max(
                    lval[2],
                    lval[3] + rval[2]
                ),
                lval[3] + rval[3]
            };
        }

        static TrnTp get_transition(TrnTp val)
        {
            return val;
        }
        static TrnTp get_transition(TrnTp lval, TrnTp rval)
        {
            return update(lval, rval);
        }

        static RetTp get(TrnTp val)
        {
            return *max_element(val.begin(), val.end());
        }
    };

    template <typename Tp>
    struct in_range_count
    {
        using DatTp = vector<Tp>;
        using TrnTp = Tp;
        using RetTp = Tp;

        static const DatTp update(const RetTp& val)
        {
            return {val};
        }
        static const DatTp update(const DatTp& lval, const DatTp& rval)
        {
            const int n = lval.size();
            const int m = rval.size();

            DatTp result;

            int i = 0, j = 0;
            while (i < n && j < m)
            {
                if (lval[i] < rval[j])
                    result.push_back(lval[i++]);
                else
                    result.push_back(rval[j++]);
            }

            for (; i < n; i++)
                result.push_back(lval[i]);
            for (; j < m; j++)
                result.push_back(rval[j]);

            return result;
        }

        static const TrnTp get_transition(const DatTp& val, Tp l, Tp r)
        {
            return l <= val[0] && val[0] <= r;
        }
        static const TrnTp get_transition(const TrnTp& lval, const DatTp& rval, Tp l, Tp r)
        {
            if (l > r)
                return lval;
            return lval + upper_bound(rval.begin(), rval.end(), r) - lower_bound(rval.begin(), rval.end(), l);
        }
        static const TrnTp get_transition(const DatTp& lval, const TrnTp& rval, Tp l, Tp r)
        {
            return get_transition(rval, lval, l, r);
        }
        static const TrnTp get_transition(const RetTp& lval, const RetTp& rval, ...)
        {
            return lval + rval;
        }

        static const RetTp get(TrnTp val)
        {
            return val;
        }
    };
}

#endif /* _SEGMENT_TREE_VARIANT */

#ifndef _SEGMENT_TREE
#define _SEGMENT_TREE 1

template <typename Var=segment_tree_variant::sum<long long>>
class segment_tree
{
    using DatTp = typename Var::DatTp;
    using TrnTp = typename Var::TrnTp;
    using RetTp = typename Var::RetTp;

public:
    const int             size, height;
    vector<vector<DatTp>> data;

private:
    template <typename... Args>
    const TrnTp get_transition(int li, int ri, Args... args) const
    {
        if (li == ri)
            return Var::get_transition(data.back()[li], args...);

        TrnTp lval = Var::get_transition(data.back()[li], args...);
        TrnTp rval = Var::get_transition(data.back()[ri], args...);

        for (int h = height - 1; li / 2 < ri / 2; li /= 2, ri /= 2, h--)
        {
            if (!(li & 1))
                lval = Var::get_transition(lval, data[h][li + 1], args...);
            if (ri & 1)
                rval = Var::get_transition(data[h][ri - 1], rval, args...);
        }
        return Var::get_transition(lval, rval, args...);
    }

public:
    template <typename... Args>
    void update(int i, const RetTp& val, Args... args)
    {
        DatTp cval = Var::update(val, args...);
        data.back()[i] = cval;
        for (int h = height - 1, mask = size; h > 0; h--, i /= 2)
        {
            if (!(i & 1))
            {
                data[h - 1][i / 2] = i < data[h].size() - 1 ? Var::update(cval, data[h][i + 1], args...) : cval;
                cval = data[h - 1][i / 2];
            }
            else
            {
                data[h - 1][i / 2] = Var::update(data[h][i - 1], cval, args...);
                cval = data[h - 1][i / 2];
            }
        }
    }

    template <typename... Args>
    const RetTp get(int li, int ri, Args... args) const
    {
        return Var::get(get_transition(li, ri, args...));
    }

public:
    template <typename It>
    segment_tree(It begin, It end)
        : size(end - begin), height(size > 1 ? 33 - __builtin_clz(size - 1) : 1)
    {
        data        = vector<vector<DatTp>>(height);
        data.back() = vector<DatTp>(size);

        int i = 0;
        for (auto it = begin; it != end; it++)
            data.back()[i++] = Var::update(*it);

        int mask = size;
        for (int i = height - 2; i >= 0; i--)
        {
            data[i].resize((mask + 1) / 2);
            for (int j = mask / 2 - 1; j >= 0; j--)
            {
                data[i][j] = Var::update
                (
                    data[i + 1][j * 2],
                    data[i + 1][j * 2 + 1]
                );
            }
            if (mask & 1)
                data[i].back() = data[i + 1].back();
            
            ++mask /= 2;
        }
    }
};

#endif /* _SEGMENT_TREE */

#pragma endregion
