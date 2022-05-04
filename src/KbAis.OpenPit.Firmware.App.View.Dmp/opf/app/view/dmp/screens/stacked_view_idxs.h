#ifndef STACKED_VIEW_IDXS_H
#define STACKED_VIEW_IDXS_H

namespace opf::app::view::dmp {

struct stacked_view_idxs {

    stacked_view_idxs() = delete;

    static inline const int MAIN { 0 };

    static inline const int DIAG { 1 };

    static inline const int INFO { 2 };

    static inline const int STOP { 3 };

};

}

#endif // STACKED_VIEW_IDXS_H
