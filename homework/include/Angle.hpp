class Angle
{
    private:
     double v0;
     double g;

    public:
     Angle(double init_v0,double grav);
     double solver(double d);
};