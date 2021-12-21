public class Lanternfish
{
  private int timer;

  public Lanternfish(int startTimer)
  {
    timer = startTimer;
  }

  boolean aDayHasPassed()
  {
    timer--;
    if (timer < 0) {
      timer = 6;
      return true;
    }
    return false;
  }
}

