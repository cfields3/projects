import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.Random;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.security.PublicKey;
import java.security.KeyFactory;
import java.security.spec.X509EncodedKeySpec;
import java.security.GeneralSecurityException;
import java.util.Base64;

/**
 * Code for CSC 246 Assignment 6, Problem 1. A server that
 * keeps up with a public key for each user, scrabble word
 * scores for all users.
 * 
 * @author Christopher Fields (cwfields)
 */
public class Server {

  /** Port number used by the server */
  public static final int PORT_NUMBER = 26022;

  /** Maximum length of a username. */
  public static int NAME_MAX = 10;

  /** Maximum word length */
  private static int WORD_LIMIT = 26;

  /** Record for an individual user. */
  private static class UserRec {
    // Name of this user.
    String name;

    // This user's public key.
    PublicKey publicKey;
  }

  /** List of all the user records. */
  private ArrayList< UserRec > userList = new ArrayList< UserRec >();

  /** Representation of the current Scrabble board */
  private char currentBoard[][];

  /** List of the valid words read through file */
  private ArrayList< String > validWords = new ArrayList< String >();

  /** Whether or not a valid words file was provided */
  private boolean hasWordsList = false;

  private static void usage() {
    System.err.println( "usage: Server <rows> <cols>" );
    System.exit( 1 );
  }

  /** Read the list of all users and their public keys. */
  private void readUsers() throws Exception {
    Scanner input = new Scanner( new File( "passwd.txt" ) );
    while ( input.hasNext() ) {
      // Create a record for the next user.
      UserRec rec = new UserRec();
      rec.name = input.next();

      // Get the key as a string of hex digits and turn it into a byte array.
      String base64Key = input.nextLine().trim();
      byte[] rawKey = Base64.getDecoder().decode( base64Key );
    
      // Make a key specification based on this key.
      X509EncodedKeySpec pubKeySpec = new X509EncodedKeySpec( rawKey );

      // Make an RSA key based on this specification
      KeyFactory keyFactory = KeyFactory.getInstance( "RSA" );
      rec.publicKey = keyFactory.generatePublic( pubKeySpec );

      // Add this user to the list of all users.
      userList.add( rec );
    }
  }

  /** Utility function to read a length then a byte array from the
      given stream.  TCP doesn't respect message boundaraies, but this
      is essientially a technique for marking the start and end of
      each message in the byte stream.  As a public, static method,
      this can also be used by the client. */
  public static byte[] getMessage( DataInputStream input ) throws IOException {
    int len = input.readInt();
    byte[] msg = new byte [ len ];
    input.readFully( msg );
    return msg;
  }

  /** Function analogous to the previous one, for sending messages. */
  public static void putMessage( DataOutputStream output, byte[] msg ) throws IOException {
    // Write the length of the given message, followed by its contents.
    output.writeInt( msg.length );
    output.write( msg, 0, msg.length );
    output.flush();
  }

  /**
   * Checks if a given word is valid; a word is valid if
   * it matches one of the words given in validWords
   * (from Assignment 5 extra credit).
   *
   * @param word the word to check if is valid
   * @return whether or not the word is valid
  */
  private boolean checkWordValid( String word ) {
    for ( int i = 0; i < validWords.size(); i++ )
      if ( word.equals( validWords.get( i ) ) )
        return true;
    return false;
  }
  
  /**
   * Checks if a given board is valid. Ensures that all
   * horizontal and vertical sequences of two or more
   * characters make up a valid word (from Assignment 5 
   * extra credit).
   * 
   * @param board the board to check validity of
   * @return whether or not the board is valid
  */
  private boolean checkBoardValid( char[][] board ) {
    int rows = currentBoard.length;
    int columns = currentBoard[ 0 ].length;

    // Check for Valid Rows
    for ( int row = 0; row < rows; row++ ) {
      for ( int column = 0; column < columns; column++ ) {
        if ( board[ row ][ column ] != 0 ) { // We found a potential start of a word
          StringBuilder word = new StringBuilder();
          // Continue to iterate through board until empty character or end of board
          while ( column < columns && board[ row ][ column ] != 0 ) {
            word.append( board[ row ][ column ] );
            column++;
          }
          // Check all sequences of two or more characters for valid (don't check 1 letter sequences)
          if ( word.toString().length() >= 2 && !checkWordValid( word.toString() ) )
            return false;
        }
      }
    }

    // Check for Valid Columns (similar but iterate horizontally)
    for ( int column = 0; column < columns; column++ ) {
      for ( int row = 0; row < rows; row++ ) {
        if ( board[ row ][ column ] != 0 ) { // We found a potential start of a word
          StringBuilder word = new StringBuilder();
          // Continue to iterate through board until empty character or end of board
          while ( row < rows && board[ row ][ column ] != 0 ) {
            word.append( board[ row ][ column ] );
            row++;
          }
          // Check all sequences of two or more characters for valid (don't check 1 letter sequences)
          if ( word.toString().length() >= 2 && !checkWordValid( word.toString() ) )
            return false;
        }
      }
    }

    return true;
  }

  /**
   * Helper method for the command "across." Places a word with its
   * left end at the given row and column and extending to the right.
   * If the command is invalid (word extends off the board, contains
   * characters other that lowercase letters), it will return false.
   * 
   * @param r the row to start the word at
   * @param c the column to start the word at
   * @param word the word to add to the board
   * @return whether or not the word was added successfully
  */
  private synchronized boolean across( int r, int c, String word ) {
    // Check if word is too long (above WORD_LIMIT) or too short (below 1 character)
    if ( word.length() > WORD_LIMIT || word.length() < 1 )
      return false;
    
    // Check if word would extend off and/or starts off board
    if ( c + word.length() > currentBoard[ 0 ].length || r >= currentBoard.length ) {
      return false;
    }

    // Create an updated board to store local changes (before copying to currentBoard)
    char[][] updatedBoard = new char[ currentBoard.length ][ currentBoard[ 0 ].length ];
    for ( int i = 0; i < currentBoard.length; i++ ) {
      for ( int j = 0; j < currentBoard[ 0 ].length; j++ ) {
        updatedBoard[ i ][ j ] = currentBoard[ i ][ j ];
      }
    }

    // Iterate through each character of the word
    for ( int i = 0; i < word.length(); i++ ) {
      if ( !Character.isLowerCase( word.charAt( i ) ) )
        return false;
      
      // Check if character is able to be places in the specified slot
      if ( updatedBoard[ r ][ c + i ] == 0 || updatedBoard[ r ][ c + i ] == word.charAt( i ) ) {
        updatedBoard[ r ][ c + i ] = word.charAt( i );
      } else {
        return false;
      }
    }

    // Check if the board is valid, only if there was a words file provided
    if ( hasWordsList && !checkBoardValid( updatedBoard ) )
      return false;

    // The board is valid - copy the updatedBoard to the currentBoard
    for ( int i = 0; i < currentBoard.length; i++ ) {
      for ( int j = 0; j < currentBoard[ 0 ].length; j++ ) {
        currentBoard[ i ][ j ] = updatedBoard[ i ][ j ];
      }
    }

    return true;
  }

  /**
   * Helper method for the command "down." Places a word with its
   * top end at the given row and column and extending downwards.
   * If the command is invalid (word extends off the board, contains
   * characters other that lowercase letters), it will return false.
   * 
   * @param r the row to start the word at
   * @param c the column to start the word at
   * @param word the word to add to the board
   * @return whether or not the word was added successfully
  */
  private synchronized boolean down( int r, int c, String word ) {
    // Check if word is too long (above WORD_LIMIT) or too short (below 1 character)
    if ( word.length() > WORD_LIMIT || word.length() < 1 )
      return false;
    
    // Check if word would extend off and/or starts off board
    if ( r + word.length() > currentBoard.length || c >= currentBoard[ 0 ].length ) {
      return false;
    }

    // Create an updated board to store local changes (before copying to currentBoard)
    char[][] updatedBoard = new char[ currentBoard.length ][ currentBoard[ 0 ].length ];
    for ( int i = 0; i < currentBoard.length; i++ ) {
      for ( int j = 0; j < currentBoard[ 0 ].length; j++ ) {
        updatedBoard[ i ][ j ] = currentBoard[ i ][ j ];
      }
    }

    // Iterate through each character of the word
    for ( int i = 0; i < word.length(); i++ ) {
      if ( !Character.isLowerCase( word.charAt( i ) ) )
        return false;
      
      // Check if character is able to be places in the specified slot
      if ( updatedBoard[ r + i ][ c ] == 0 || updatedBoard[ r + i ][ c ] == word.charAt( i ) ) {
        updatedBoard[ r + i ][ c ] = word.charAt( i );
      } else {
        return false;
      }
    }

    // Check if the board is valid, only if there was a words file provided
    if ( hasWordsList && !checkBoardValid( updatedBoard ) )
      return false;

    // The board is valid - copy the updatedBoard to the currentBoard
    for ( int i = 0; i < currentBoard.length; i++ ) {
      for ( int j = 0; j < currentBoard[ 0 ].length; j++ ) {
        currentBoard[ i ][ j ] = updatedBoard[ i ][ j ];
      }
    }

    return true;
  }

  /**
   * Helper method for the command "board." Returns a String for
   * the printed output of the board command in the Client. Will
   * include a border and print spaces for all missing characters.
   *
   * @return the String to send to the Client as output
  */
  private synchronized String board() {
    StringBuilder output = new StringBuilder();

    // Print top border.
    output.append( "+" );
    for ( int i = 0; i < currentBoard[ 0 ].length; i++ )
      output.append( "-" );
    output.append( "+\n" );

    // Print the contents (and side borders) of the board.
    for ( int i = 0; i < currentBoard.length; i++ ) {
      output.append( "|" );

      for ( int j = 0; j < currentBoard[ 0 ].length; j++ ) {
        if ( currentBoard[ i ][ j ] == 0 ) {
          output.append( " " );
        } else {
          output.append( currentBoard[ i ][ j ] );
        }
      }

      output.append( "|\n" );
    }

    // Print the bottom border.
    output.append( "+" );
    for ( int i = 0; i < currentBoard[ 0 ].length; i++ ) {
      output.append( "-" );
    }
    output.append( "+\n" );

    return output.toString();
  }

  /**
   * Helper method to look for a "words" file in the
   * current director and read all words into the validWords
   * file if it exists (from Assignment 5 extra credit).
  */
  private void readValidWords() {
    try {
      Scanner input = new Scanner( new File( "words" ) );
      hasWordsList = true;

      // Add each word to the validWords list
      while ( input.hasNextLine() )
        validWords.add( input.nextLine() );
    } catch ( FileNotFoundException e ) { // If the words file does not exist
      hasWordsList = false;
    }
  }

  /** Handle interaction with our client. */
  public void handleClient( Socket sock ) {
    try {
      // Get formatted input/output streams for this thread.  These can read and write
      // strings, arrays of bytes, ints, lots of things.
      DataOutputStream output = new DataOutputStream( sock.getOutputStream() );
      DataInputStream input = new DataInputStream( sock.getInputStream() );
      
      // Get the username.
      String username = input.readUTF();

      // Make a random sequence of bytes to use as a challenge string.
      Random rand = new Random();
      byte[] challenge = new byte [ 16 ];
      rand.nextBytes( challenge );

      // Make a session key for communiating over AES.  We use it later, if the
      // client successfully authenticates.  Is this a safe way to generate
      // a random key?  Someone should check on that.
      byte[] sessionKey = new byte [ 16 ];
      rand.nextBytes( sessionKey );

      // Find this user.  We don't need to synchronize here, since the set of users never
      // changes.
      UserRec rec = null;
      for ( int i = 0; rec == null && i < userList.size(); i++ )
        if ( userList.get( i ).name.equals( username ) )
          rec = userList.get( i );

      // Did we find a record for this user?
      if ( rec != null ) {
        // Make sure the client encrypted the challenge properly.
        Cipher RSADecrypter = Cipher.getInstance( "RSA" );
        RSADecrypter.init( Cipher.DECRYPT_MODE, rec.publicKey );
          
        Cipher RSAEncrypter = Cipher.getInstance( "RSA" );
        RSAEncrypter.init( Cipher.ENCRYPT_MODE, rec.publicKey );
          
        // Send the client the challenge.
        putMessage( output, challenge );
          
        // Get back the client's encrypted challenge.
        byte[] encryptedChallenge = getMessage( input );

        // Make sure the client properly encrypted the challenge.
        byte[] decryptedChallenge = RSADecrypter.doFinal( encryptedChallenge );
        if ( !Arrays.equals( challenge, decryptedChallenge ) ) {
          sock.close();
        }

        // Send the client our session key (encrypted)
        byte[] encryptedSessionKey = RSAEncrypter.doFinal( sessionKey );
        putMessage( output, encryptedSessionKey );

        // Make a key object for the sessionKey.
        SecretKey sessionKeySpec = new SecretKeySpec( sessionKey, "AES" );

        // Make a cipher object that will encrypt using the sessionKey.
        Cipher AESEncrypter = Cipher.getInstance( "AES/ECB/PKCS5Padding" );
        AESEncrypter.init( Cipher.ENCRYPT_MODE, sessionKeySpec );

        // Make another cipher object that will decrypt using the sessionKey.
        Cipher AESDecrypter = Cipher.getInstance( "AES/ECB/PKCS5Padding" );
        AESDecrypter.init( Cipher.DECRYPT_MODE, sessionKeySpec );

        // Get the first client command
        String request = new String( AESDecrypter.doFinal( getMessage( input ) ) );
        String[] requestArray = request.split( " " );
        String command = requestArray[ 0 ];

        // All requests start with a verb.
        while ( !command.equals( "quit" ) ) {
          StringBuilder reply = new StringBuilder();

          if ( command.equals( "across" ) ) {
            if ( requestArray.length == 4 ) { // Ensure that 4 arguments were given
              try {
                // Get all arguments given
                int row = Integer.parseInt( requestArray[ 1 ] );
                int column = Integer.parseInt( requestArray[ 2 ] );
                String word = requestArray[ 3 ];

                reply.append( across( row, column, word ) ? "No Output" : "Invalid command\n" );
              } catch ( Exception e ) { // If an argument is of the wrong type
                reply.append( "Invalid command\n" );
              }
            } else { // If the wrong number of arguments are given
              reply.append( "Invalid command\n" );
            }
          } else if ( command.equals( "down" ) ) {
            if ( requestArray.length == 4 ) { // Ensure that 4 arguments were given
              try {
                int row = Integer.parseInt( requestArray[ 1 ] );
                int column = Integer.parseInt( requestArray[ 2 ] );
                String word = requestArray[ 3 ];

                reply.append( down( row, column, word ) ? "No Output" : "Invalid command\n" );
              } catch ( Exception e ) { // If an argument is of the wrong type
                reply.append( "Invalid command\n" );
              }
            } else { // If the wrong number of arguments are given
              reply.append( "Invalid command\n" );
            }
          } else if ( command.equals( "board" ) ) {
            if ( requestArray.length == 1 ) { // Ensure that a single argument was given
              reply.append( board() );
            } else { // If additional arguments were given
              reply.append( "Invalid command\n" );
            }
          } else { // If the command does not match one specified
            reply.append( "Invalid command\n" );
          }
              
          // Send the reply back to our client
          putMessage( output, AESEncrypter.doFinal( reply.toString().getBytes() ) );
              
          // Get the next command.
          request = new String( AESDecrypter.doFinal( getMessage( input ) ) );
          requestArray = request.split( " " );
          command = requestArray[ 0 ];
        }
      }
    } catch ( IOException e ) {
      System.out.println( "IO Error: " + e );
    } catch( GeneralSecurityException e ){
      System.err.println( "Encryption error: " + e );
    } finally {
      try {
        // Close the socket on the way out.
        sock.close();
      } catch ( Exception e ) {
      }
    }
  }

  /**
   * Class for creating a new Thread to handle each client.
   * Runs the handleClient() method with the given Socket.
   *
   * @author Christopher Fields (cwfields)
  */
  private class ClientHandler extends Thread {

    private Socket sock;

    public ClientHandler( Socket sock ) {
      this.sock = sock;
    }

    public void run() {
      handleClient( sock );
    }

  }

  /** Esentially, the main method for our server, as an instance method
      so we can access non-static fields. */
  private void run( String[] args ) {
    // Print usage message & exit on invalid command-line arguments.
    if ( args.length != 2 )
      usage();

    // Reads rows and columns, exiting with usage message if are not integers > 0
    try {
      int rows = Integer.parseInt( args[ 0 ] );
      int columns = Integer.parseInt( args[ 1 ] );

      if ( rows <= 0 || columns <= 0 )
        usage();

      // Initialize the board
      currentBoard = new char[ rows ][ columns ];
    } catch ( NumberFormatException e ) {
      usage();
    }

    // Read in data from words file, if found
    readValidWords();

    ServerSocket serverSocket = null;
    
    // One-time setup.
    try {
      // Read the map and the public keys for all the users.
      readUsers();

      // Open a socket for listening.
      serverSocket = new ServerSocket( PORT_NUMBER );
    } catch( Exception e ){
      System.err.println( "Can't initialize server: " + e );
      e.printStackTrace();
      System.exit( 1 );
    }
     
    // Keep trying to accept new connections and serve them.
    while( true ){
      try {
        // Try to get a new client connection.
        Socket sock = serverSocket.accept();

        // Handle interaction with this client.
        ClientHandler clientHandler = new ClientHandler( sock );
        clientHandler.start();
      } catch( IOException e ){
        System.err.println( "Failure accepting client " + e );
      }
    }
  }

  public static void main( String[] args ) {
    // Make a server object, so we can use non-static fields and methods.
    Server server = new Server();
    server.run( args );
  }

}
