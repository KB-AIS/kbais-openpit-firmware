#ifndef STACKED_VIEW_H
#define STACKED_VIEW_H

class i_stacked_view {

public:
    virtual ~i_stacked_view() noexcept = default;

    virtual int get_view_idx() const noexcept = 0;

    //virtual void on_start() noexcept = 0;

    //virtual void on_pause() noexcept = 0;

};

#endif // STACKED_VIEW_H
