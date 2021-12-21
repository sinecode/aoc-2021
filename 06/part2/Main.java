import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Main
{
  public static void main(String[] args) throws IOException
  {
    if (args.length == 0) {
      System.err.println("Please provide the input file as argument");
      System.exit(1);
    }

    String inputFileContent = Files.readString(Paths.get(args[0]));

    Map<Integer, Long> currentDay = new HashMap<>();
    for (String timerValue : inputFileContent.strip().split(",")) {
      int startTimer = Integer.valueOf(timerValue);
      if (currentDay.containsKey(startTimer)) {
        currentDay.put(startTimer, currentDay.get(startTimer) + 1L);
      } else {
        currentDay.put(startTimer, 1L);
      }
    }

    int numDays = 256;

    for (int day = 0; day < numDays; day++) {
      Map<Integer, Long> nextDay = new HashMap<>();
      for (Map.Entry<Integer, Long> entry : currentDay.entrySet()) {
        int newTimer = entry.getKey() - 1;
        if (newTimer < 0) {
          // Add child
          if (nextDay.containsKey(8)) {
            nextDay.put(8, nextDay.get(8) + entry.getValue());
          } else {
            nextDay.put(8, entry.getValue());
          }

          // Add parent
          if (nextDay.containsKey(6)) {
            nextDay.put(6, nextDay.get(6) + entry.getValue());
          } else {
            nextDay.put(6, entry.getValue());
          }

        } else {
          if (nextDay.containsKey(newTimer)) {
            nextDay.put(newTimer, nextDay.get(newTimer) + entry.getValue());
          } else {
            nextDay.put(newTimer, entry.getValue());
          }
        }
      }
      currentDay = nextDay;
    }

    long totalLanternfishes = 0L;
    for (Long v : currentDay.values()) {
      totalLanternfishes += v;
    }
    System.out.println(totalLanternfishes);
  }
}

