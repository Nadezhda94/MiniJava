class Factorial{
    public static void main(String[] a){
    System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {

    public int ComputeFac(int num){
    int num_aux ;
    boolean b;

    num_aux = 1 ;
    if (num_aux < 2)
        num_aux = 1 ;
    else
        num_aux = num * (this.ComputeFac((this.ComputeFac(1)))) ;
    return num_aux ;
    }

}
