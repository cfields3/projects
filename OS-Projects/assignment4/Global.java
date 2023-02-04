import java.util.Random;

/**
 * Code for CSC 246 Assignment 4, Problem 2. Simulates
 * a kitchen with various chefs that need to use various
 * appliances simultaneously in order to cook. Modification
 * of Kitchen.java to include deadlock preventions by ensuring
 * that only one chef can cook at a time.
 * 
 * @author Christopher Fields (cwfields)
 */
public class Global {
  /** To tell all the chefs when they can quit running. */
  private static boolean running = true;

  /** Superclass for all chefs.  Contains methods to cook and rest and
      keeps a record of how many dishes were prepared. */
  private static class Chef extends Thread {
    /** Number of dishes prepared by this chef. */
    private int dishCount = 0;

    /** Source of randomness for this chef. */
    private Random rand = new Random();

    /** Called after the chef has locked all the required appliances and is
        ready to cook for about the given number of milliseconds. */
    protected void cook( int duration ) {
      System.out.printf( "%s is cooking\n", getClass().getSimpleName() );
      try {
        // Wait for a while (pretend to be cooking)
        Thread.sleep( rand.nextInt( duration / 2 ) + duration / 2 );
      } catch ( InterruptedException e ) {
      }
      dishCount++;
    }

    /** Called between dishes, to let the chef rest before cooking another dish. */
    protected void rest( int duration ) {
      System.out.printf( "%s is resting\n", getClass().getSimpleName() );
      try {
        // Wait for a while (pretend to be resting)
        Thread.sleep( rand.nextInt( duration / 2 ) + duration / 2 );
      } catch ( InterruptedException e ) {
      }
    }
  }
  
  // An object representing the lock what cooks can cook at a time.
  // Only allows one cook to cook at a time and therefore prevents deadlock.
  private static Object cooking = new Object();

  /** Mandy is a chef needing 105 milliseconds to prepare a dish. */
  private static class Mandy extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 105 );
        }
        
        rest( 25 );
      }
    }
  }

  /** Edmund is a chef needing 30 milliseconds to prepare a dish. */
  private static class Edmund extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 30 );
        }

        rest( 25 );
      }
    }
  }

  /** Napoleon is a chef needing 60 milliseconds to prepare a dish. */
  private static class Napoleon extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 60 );
        }
            
        rest( 25 );
      }
    }
  }

  /** Prudence is a chef needing 15 milliseconds to prepare a dish. */
  private static class Prudence extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 15 );
        }

        rest( 25 );
      }
    }
  }

  /** Kyle is a chef needing 45 milliseconds to prepare a dish. */
  private static class Kyle extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 45 );
        }

        rest( 25 );
      }
    }
  }

  /** Claire is a chef needing 15 milliseconds to prepare a dish. */
  private static class Claire extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 15 );
        }
          
        rest( 25 );
      }
    }
  }

  /** Lucia is a chef needing 15 milliseconds to prepare a dish. */
  private static class Lucia extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 15 );
        }

        rest( 25 );
      }
    }
  }

  /** Marcos is a chef needing 60 milliseconds to prepare a dish. */
  private static class Marcos extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 60 );
        }

        rest( 25 );
      }
    }
  }

  /** Roslyn is a chef needing 75 milliseconds to prepare a dish. */
  private static class Roslyn extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 75 );
        }

        rest( 25 );
      }
    }
  }

  /** Stephenie is a chef needing 30 milliseconds to prepare a dish. */
  private static class Stephenie extends Chef {
    public void run() {
      while ( running ) {
        synchronized ( cooking ) {
          cook( 30 );
        }

        rest( 25 );
      }
    }
  }

  public static void main( String[] args ) throws InterruptedException {
    // Make a thread for each of our chefs.
    Chef chefList[] = {
      new Mandy(),
      new Edmund(),
      new Napoleon(),
      new Prudence(),
      new Kyle(),
      new Claire(),
      new Lucia(),
      new Marcos(),
      new Roslyn(),
      new Stephenie(),
    };

    // Start running all our chefs.
    for ( int i = 0; i < chefList.length; i++ )
      chefList[ i ].start();

    // Let the chefs cook for a while, then ask them to stop.
    Thread.sleep( 10000 );
    running = false;

    // Wait for all our chefs to finish, and collect up how much
    // cooking was done.
    int total = 0;
    for ( int i = 0; i < chefList.length; i++ ) {
      chefList[ i ].join();
      System.out.printf( "%s cooked %d dishes\n",
                         chefList[ i ].getClass().getSimpleName(),
                         chefList[ i ].dishCount );
      total += chefList[ i ].dishCount;
    }
    System.out.printf( "Total dishes cooked: %d\n", total );
  }
}
