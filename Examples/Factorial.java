class Factorial{
    public static void main(String[] a){
    System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {

    public int ComputeFac(int num){
    int num_aux ;

    num_aux = 1 ;
    if (num_aux < 2)
        num_aux = 1 ;
    else
        num_aux = num * (this.ComputeFac((this.ComputeFac()))) ;
    return num_aux ;
    }

}
