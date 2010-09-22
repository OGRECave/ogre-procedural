package procedural;
import java.io.File;
import java.io.FileInputStream;


public class ProceduralCodeGen {
public static void main(String[] args) throws Exception {
	String className = "TubeGenerator";
	File f = new File("input.txt");
	FileInputStream fis = new FileInputStream(f);
	byte[] buf = new byte[(int)f.length()];
	fis.read(buf);
	String input = new String(buf);
	String tokens[] = input.split(";");
	for (String token : tokens) {
		token = token.trim();
		int index = token.lastIndexOf(" ");
		String key = token.substring(0, index);
		String value = token.substring(index +1, token.length());
		String capitalValue = value.substring(0, 1).toUpperCase() + value.substring(1);
		System.out.println("\tinline "+className+" & set"+capitalValue+"("+key+" "+value+")\n\t{\n\t\tthis->"+value+" = "+value+";\n\t\treturn *this;\n\t}\n");
	}
}
}
