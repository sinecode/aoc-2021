import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Stack;

public class Main
{
  public static void main(String[] args) throws IOException
  {
    Main m = new Main();
    m.run(args);
  }

  private static final Map<Character, Character> PARENTHESIS_MAP =
      Map.of('(', ')', '[', ']', '{', '}', '<', '>');
  private static final Map<Character, Long> POINT_MAP = Map.of(')', 1l, ']', 2l, '}', 3l, '>', 4l);

  private void run(String[] args) throws IOException
  {
    if (args.length < 1) {
      System.err.println("Please provide the input file as argument");
      System.exit(1);
    }
    String[] lines = readLines(Paths.get(args[0]));
    List<Long> lineScores = new ArrayList<>();
    for (String line : lines) {
      Stack<Character> parenthesisStack = new Stack<>();
      CharacterIterator iter = new StringCharacterIterator(line);
      boolean invalidLine = false;
      while (iter.current() != CharacterIterator.DONE) {
        if (PARENTHESIS_MAP.keySet().contains(iter.current())) {
          parenthesisStack.push(iter.current());
        } else {
          char expectedParenthesis = PARENTHESIS_MAP.get(parenthesisStack.pop());
          if (iter.current() != expectedParenthesis) {
            invalidLine = true;
            break; // skip the invalid lines
          }
        }
        iter.next();
      }
      if (invalidLine)
        continue; // skip the invalid lines
      long lineScore = 0;
      int i = 0;
      while (!parenthesisStack.empty()) {
        char expectedParenthesis = PARENTHESIS_MAP.get(parenthesisStack.pop());
        lineScore = (lineScore * 5) + POINT_MAP.get(expectedParenthesis);
        i++;
      }
      lineScores.add(lineScore);
    }
    Collections.sort(lineScores);
    System.out.println(lineScores.get(lineScores.size() / 2));
  }

  private String[] readLines(Path path) throws IOException
  {
    return Files.readString(path).trim().split("\n");
  }
}
