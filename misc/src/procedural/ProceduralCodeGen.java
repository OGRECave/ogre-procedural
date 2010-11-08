package procedural;

import java.io.File;
import java.io.FileInputStream;

/**
 * This class takes a class name and and input file as input and outputs getter code for Procedural Generators
 * 
 * The input file is composed as follow (exemple) :
 *  int numSegSection;  
 *  int numSegCircle;
 *  float radius;
 * 
 * @author Mike
 *
 */
public class ProceduralCodeGen {
	public static void main(String[] args) throws Exception {
		if (args.length<2) {
			System.err.println("Procedural code generator takes two parameters :");
			System.err.println(" 1. class name");
			System.err.println(" 2. input file");			
		}		
		
		String className = args[0];
		File f = new File(args[1]);
		FileInputStream fis = new FileInputStream(f);
		byte[] buf = new byte[(int) f.length()];
		fis.read(buf);
		String input = new String(buf);
		String tokens[] = input.split(";");
		for (String token : tokens) {
			token = token.trim();
			int index = token.lastIndexOf(" ");
			String key = token.substring(0, index);
			String value = token.substring(index + 1, token.length());
			String capitalValue = value.substring(0, 1).toUpperCase() + value.substring(1);
			System.out.println("\tinline " + className + " & set" + capitalValue + "(" + key + " " + value + ")\n\t{\n\t\tthis->" + value + " = " + value
					+ ";\n\t\treturn *this;\n\t}\n");
		}
	}
}
