class test08 {
	public static void main(String[] a) {
		System.out.println(new Body().go());
	}
}

class Body {
	public int value(int i, int j, int k) {
		return i+j+k;
	}

public int go() {
		return this.value(1,1,1) + this.value(2,2,2);
	}
}
