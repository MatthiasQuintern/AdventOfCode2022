import java.io.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.lang.Math;
import java.lang.Thread;

public class Main {
    static int instructionCounter = 1;
    static int registerX = 1;
    static int signalStrenghts = 0;

    public static String getCurrentPixel() {
        String s = "";
        int crtPos = (instructionCounter - 1) % 40;
        // System.out.printf("crtpos: %d\n", crtPos);
        if (Math.abs(registerX - 1 - crtPos) <= 1) { s = "█"; }
        else { s = "░"; }
        if (instructionCounter % 40 == 0) { s += "\r\n"; }
        return s;
    }
    public static void instructionFinished() {
        System.out.print(getCurrentPixel());
        instructionCounter++;
        if (instructionCounter >= 20 && (instructionCounter - 20) % 40 == 0) {
            signalStrenghts += registerX * instructionCounter;
        }
        try {
          Thread.sleep(50);
        }
        catch (InterruptedException ignore) {}
    }

    public static void main(String[] args) throws IOException {
        String input = Files.readString(Path.of(args[0]));
        String separator = System.getProperty("line.separator");
        int index = 0;
        while (true) {
          int nextIndex = input.indexOf(separator, index + separator.length());
          if (nextIndex < 0) { break; };
          String line = input.substring(index, nextIndex);
          
          instructionFinished();
          if (!line.equals("noop")) {
            int val = Integer.parseInt(line.split(" ")[1]);
            registerX += val;
            instructionFinished();
          }
          index = nextIndex + 1;
        }
        System.out.printf("The sum of the signal strengths is: %d\n", signalStrenghts);
    }
}
