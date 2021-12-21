import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.LinkedList;
import java.util.List;

public class Main
{
  public static void main(String[] args) throws IOException
  {
    if (args.length == 0) {
      System.err.println("Please provide the input file as argument");
      System.exit(1);
    }

    String inputFileContent = Files.readString(Paths.get(args[0]));
    List<Lanternfish> lanternfishes = new LinkedList<>();
    for (String timerValue : inputFileContent.strip().split(",")) {
      lanternfishes.add(new Lanternfish(Integer.parseInt(timerValue)));
    }

    int numDays = 80;

    for (int day = 0; day < numDays; day++) {
      List<Lanternfish> newLanternfishes = new LinkedList<>();
      for (Lanternfish lanternfish : lanternfishes) {
        boolean shouldReproduce = lanternfish.aDayHasPassed();
        if (shouldReproduce) {
          newLanternfishes.add(new Lanternfish(8));
        }
      }
      lanternfishes.addAll(newLanternfishes);
    }

    System.out.println(lanternfishes.size());
  }
}
