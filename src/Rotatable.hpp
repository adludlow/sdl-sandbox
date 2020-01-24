#ifndef ROTATABLE_HPP
#define ROTATABLE_HPP

class Rotatable {
  public:
    virtual void rotate( bool keepHeading = false ) = 0;
};

#endif
