import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.Map;
import java.util.Set;
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
  private static final Map<Character, Integer> POINT_MAP =
      Map.of(')', 3, ']', 57, '}', 1197, '>', 25137);

  private void run(String[] args) throws IOException
  {
    if (args.length < 1) {
      System.err.println("Please provide the input file as argument");
      System.exit(1);
    }
    String[] lines = readLines(Paths.get(args[0]));
    long totalPoints = 0;
    for (String line : lines) {
      Stack<Character> parenthesisStack = new Stack<>();
      CharacterIterator iter = new StringCharacterIterator(line);
      while (iter.current() != CharacterIterator.DONE) {
        if (PARENTHESIS_MAP.keySet().contains(iter.current())) {
          parenthesisStack.push(iter.current());
        } else {
          char expectedParenthesis = PARENTHESIS_MAP.get(parenthesisStack.pop());
          if (iter.current() != expectedParenthesis) {
            totalPoints += POINT_MAP.get(iter.current());
          }
        }
        iter.next();
      }
    }
    System.out.println(totalPoints);
  }

  private String[] readLines(Path path) throws IOException
  {
    return Files.readString(path).trim().split("\n");
  }
}
