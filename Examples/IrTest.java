class test08 {
	public static void main(String[] a) {
		System.out.println(new Aaa().go2());
	}
}

class Body {
	int b;
	int c;
}

class Aaa extends Body{
	public int go2() {
		return this.value(1,1,1);
	}

	public int value(int i, int j, int k) {
		b=7;
		return i+j+k;
	}

	public int go() {
			return this.value(1,1,1) + this.value(2,2,2);
		}
}
