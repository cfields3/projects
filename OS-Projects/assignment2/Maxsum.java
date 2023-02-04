import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Code for CSC 246 Assignment 2, Problem 2. Calculates
 * the maximum sum for a contiguous sequence of numbers
 * over an inputed list using a certain number of workers
 * as separate threads.
 * 
 * @author Christopher Fields (cwfields)
 */
public class Maxsum {

    /** Input sequence of values */
    private static List<Integer> vList;

    /** Initial capacity of the list */
    private static final int INIT_CAPACITY = 5;

    /** Number of workers to use */
    private static int workers = 4;

    /** Specifies whether the report flag was used */
    private static boolean report = false;

    /**
     * Prints out an error message and exits.
     * 
     * @param message error message to print
     */
    private static void fail(String message) {
        System.err.println(message);
        System.exit(1);
    }

    /**
     * Prints out a usage message and exits.
     */
    private static void usage() {
        System.out.println("usage: maxsum <workers>");
        System.out.println("       maxsum <workers> report");
        System.exit(1);
    }

    /**
     * Sets up vList by reading values from System.in.
     */
    private static void readList() {
        // Set up initial list and capacity
        vList = new ArrayList<Integer>(INIT_CAPACITY);

        // Keep reading as many values as we can
        Scanner scan = new Scanner(System.in);
        while (scan.hasNextInt()) {
            // Store the latest value in the next array slot
            vList.add(scan.nextInt());
        }
        scan.close();
    }

    /**
     * Defines a custom Thread inheriting the properties
     * of java.lang.Thread with a custom run() method.
     * Calculates the maximum sum of selected ranges based
     * on the workerID of the Thread and the total number of
     * workers.
     * 
     * @author Christopher Fields (cwfields)
     */
    static class WorkerThread extends Thread {

        /** Thread's ID used to determine which sums it should calculate */
        private int workerID;

        /** Maximum sum found by the thread, used to return to the main thread */
        private int maxSum;

        public WorkerThread(int workerID) {
            this.workerID = workerID;
        }

        @Override
        public void run() {
            // Initialize maxSum to the smallest value in list
            maxSum = vList.get(0);
            for (int i = 0; i < vList.size(); i++) {
                if (vList.get(i) < maxSum) {
                    maxSum = vList.get(i);
                }
            }

            // Calculates the maximum sum for specified worker, skipping
            // indices based on the number of workers
            for (int i = workerID; i < vList.size(); i += workers) {
                int sum = 0;
                for (int j = i; j < vList.size(); j++) {
                    sum += vList.get(j);
                    if (sum > maxSum)
                        maxSum = sum;
                }
            }

            // Print out a report, if requested
            if (report)
                System.out.println("I'm thread " + this.getId() + ". The maximum sum I found is " + maxSum + ".");
        }

    }

    /**
     * Starting point of the program. Inputs command-line arguments
     * and create an appropriate number of worker threads based
     * on those arguments. Then, waits for the worker threads to
     * finish and prints the maximum sum back to the user.
     * 
     * @param args command-line arguments
     */
    public static void main(String args[]) {
        // Parse command-line arguments
        if (args.length < 1 || args.length > 2)
            usage();

        // Check if second command line argument is an Integer
        try {
            workers = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            usage();
        }

        // Report usage with invalid number of workers
        if (workers < 1) {
            usage();
        }

        // Second command-line argument must be the word, "report"
        if (args.length == 2) {
            if (!args[1].equals("report"))
                usage();
            report = true;
        }

        readList();

        // Iterate through and create all workers, assigning them a new ID
        WorkerThread[] workerThreads = new WorkerThread[workers];
        for (int i = 0; i < workers; i++) {
            workerThreads[i] = new WorkerThread(i);
            workerThreads[i].start();
        }

        // Wait for all workers and calculate maxSum based on max values for each thread
        int maxSum = 0;
        try {
            for (int i = 0; i < workers; i++) {
                workerThreads[i].join();

                if (workerThreads[i].maxSum > maxSum) {
                    maxSum = workerThreads[i].maxSum;
                }

            }
        } catch (InterruptedException e) {
            fail("A worker thread was interrupted during join");
        }

        System.out.println("Maximum Sum: " + maxSum);
    }

}
