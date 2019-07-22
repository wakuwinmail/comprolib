//cut begin
template<typename T>
class monoid{
private:
    typedef std::function<T(T,T)> F;
    F cal;
    T id;
public:
    monoid(F f,T id):cal(f),id(id){}
    static T operator()(T a,T b){
        return cal(a,b);
    }
    static T get_id(){return id;}
};
//cut end