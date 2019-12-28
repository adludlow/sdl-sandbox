class Point {
  public:
    Point();
    Point( double x, double y );
    ~Point();

    double getX();
    void setX( double x );
    double getY();
    void setY( double y );

  private:
    double x;
    double y;
};