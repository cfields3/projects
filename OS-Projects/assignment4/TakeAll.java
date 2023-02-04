import java.util.Random;

/**
 * Code for CSC 246 Assignment 4, Problem 2. Simulates
 * a kitchen with various chefs that need to use various
 * appliances simultaneously in order to cook. Modification
 * of Kitchen.java to include deadlock avoidance by having
 * each chef lock appliances only when they are all available.
 * 
 * @author Christopher Fields (cwfields)
 */
public class TakeAll {
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

  // A boolean flag representing the lock on each appliance.
  // Setting boolean flags to true while cooking prevents two
  // chefs from trying to use the same appliance at the same time.
  private static boolean griddle = false;
  private static boolean mixer = false;
  private static boolean oven = false;
  private static boolean blender = false;
  private static boolean grill = false;
  private static boolean fryer = false;
  private static boolean microwave = false;
  private static boolean coffeeMaker = false;
  
  private static Object changeFlags = new Object();

  /** Mandy is a chef needing 105 milliseconds to prepare a dish. */
  private static class Mandy extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( microwave || coffeeMaker ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          microwave = true;
          coffeeMaker = true;
        }
        
        cook( 105 );
        
        synchronized ( changeFlags ) {
          microwave = false;
          coffeeMaker = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Edmund is a chef needing 30 milliseconds to prepare a dish. */
  private static class Edmund extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( blender || oven || mixer ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          blender = true;
          oven = true;
          mixer = true;
        }
         
        cook( 30 );
        
        synchronized ( changeFlags ) {
          blender = false;
          oven = false;
          mixer = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Napoleon is a chef needing 60 milliseconds to prepare a dish. */
  private static class Napoleon extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( blender || grill ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          blender = true;
          grill = true;
        }
        
        cook( 60 );
        
        synchronized ( changeFlags ) {
          blender = false;
          grill = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Prudence is a chef needing 15 milliseconds to prepare a dish. */
  private static class Prudence extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( coffeeMaker || microwave || griddle ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          coffeeMaker = true;
          microwave = true;
          griddle = true;
        }
        
        cook( 15 );
        
        synchronized ( changeFlags ) {
          coffeeMaker = false;
          microwave = false;
          griddle = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Kyle is a chef needing 45 milliseconds to prepare a dish. */
  private static class Kyle extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( fryer || oven ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          fryer = true;
          oven = true;
        }
        
        cook( 45 );
        
        synchronized ( changeFlags ) {
          fryer = false;
          oven = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Claire is a chef needing 15 milliseconds to prepare a dish. */
  private static class Claire extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( grill || griddle ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          grill = true;
          griddle = true;
        }
        
        cook( 15 );
        
        synchronized ( changeFlags ) {
          grill = false;
          griddle = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Lucia is a chef needing 15 milliseconds to prepare a dish. */
  private static class Lucia extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( griddle || mixer ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          griddle = true;
          mixer = true;
        }
        
        cook( 15 );
        
        synchronized ( changeFlags ) {
          griddle = false;
          mixer = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Marcos is a chef needing 60 milliseconds to prepare a dish. */
  private static class Marcos extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( microwave || fryer || blender ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          microwave = true;
          fryer = true;
          blender = true;
        }
        
        cook( 60 );
        
        synchronized ( changeFlags ) {
          microwave = false;
          fryer = false;
          blender = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Roslyn is a chef needing 75 milliseconds to prepare a dish. */
  private static class Roslyn extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( fryer || grill ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          fryer = true;
          grill = true;
        }
        
        cook( 75 );
        
        synchronized ( changeFlags ) {
          fryer = false;
          grill = false;
          
          changeFlags.notifyAll();
        }

        rest( 25 );
      }
    }
  }

  /** Stephenie is a chef needing 30 milliseconds to prepare a dish. */
  private static class Stephenie extends Chef {
    public void run() {
      while ( running ) {
        // Get the appliances this chef uses.
        synchronized ( changeFlags ) {
          while ( mixer || coffeeMaker || oven ) {
            try {
              changeFlags.wait();
            } catch (InterruptedException e) {
            }
          }
          
          mixer = true;
          coffeeMaker = true;
          oven = true;
        }
        
        cook( 30 );
        
        synchronized ( changeFlags ) {
          mixer = false;
          coffeeMaker = false;
          oven = false;
          
          changeFlags.notifyAll();
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
