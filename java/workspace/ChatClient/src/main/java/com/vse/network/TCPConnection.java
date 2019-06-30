package com.vse.network;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.Charset;

public class TCPConnection {
	public Socket getSocket() {
		return socket;
	}

	public Thread getRxThread() {
		return rxThread;
	}

	public BufferedReader getIn() {
		return in;
	}

	public BufferedWriter getOut() {
		return out;
	}

	public TCPConnectionListener getEventListener() {
		return eventListener;
	}

	private final Socket socket;
	private final Thread rxThread;
	private final BufferedReader in;
	private final BufferedWriter out;
	private final TCPConnectionListener eventListener;
	


	public TCPConnection(TCPConnectionListener eventListener, String ipAddr, int port) throws IOException {
		this(new Socket(ipAddr, port), eventListener);	
	}

	public TCPConnection(Socket socket, TCPConnectionListener eventListener) throws IOException { // create server socket/ eventListener is client socket
		this.eventListener = eventListener;
		this.socket = socket; // get server's out and in streams
		in = new BufferedReader(new InputStreamReader(socket.getInputStream(), Charset.forName("UTF-8")));
		out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), Charset.forName("UTF-8")));
		rxThread = new Thread(new Runnable() {// create thread that will listen to server


			@Override
			public void run() {
				try {
					eventListener.onConnectionReady(TCPConnection.this);
					while (!rxThread.isInterrupted()) {
//					String msg = in.readLine();
						eventListener.onReceiveString(TCPConnection.this, in.readLine());
						System.out.println("z");
						System.out.println(rxThread.isAlive() + " rxthread");
					}

				} catch (Exception e) {

				} finally {
					eventListener.onDisconnect(TCPConnection.this);
				}

			}
		});
		rxThread.start();
		System.out.println(rxThread.isAlive() + " rxthread");
	}

	public synchronized void sendString(String value) {
//		System.out.println(rxThread.isAlive() + " rxthread");
		try {
			out.write(value + "\r\n"); // \r\n = for end of a line // standart windows end of a line
			out.flush();
		} catch (Exception e) {
			System.out.println("2");
			eventListener.onException(TCPConnection.this, e);
			disconnect();
		}
	}

	public synchronized void disconnect() {
		rxThread.interrupt();
		try {
			System.out.println("1");
			socket.close();

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("1");
			eventListener.onException(TCPConnection.this, e);

		}
	}
	@Override
	public String toString() {
		return "TCPConnection: " + socket.getInetAddress() + ": " + socket.getPort();
	}
}