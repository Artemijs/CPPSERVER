using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine.UI;
public class Client : MonoBehaviour {
//	Socket serverSocket;
	UdpClient client;
	public InputField input;
	public GameObject _player;
	string _name;
	int _modCount;
	int _frameCount;
	GameObject[] _allPlayers;
	void Start(){
		_allPlayers = new GameObject[10];
		_frameCount = 0;
		_modCount = (int)(25.0f / 60.0f);
		try{
			client = new UdpClient (5555);
			client.Connect(IPAddress.Parse("192.168.1.9"), 5555);
		}
		catch(Exception e){
			Debug.Log (e.ToString ());
		}
	}

	// Use this for initialization
	/*void Start () {
		//data buffer for incoming data


		//connect to remote evice
		try{
			//establish the remote end point for a socket
			IPAddress ipAddress = IPAddress.Parse("192.168.1.9");
			IPEndPoint remoteIp = new IPEndPoint(ipAddress, 5555);

			//create tcp/ip socket
			serverSocket = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			try {
				serverSocket.Connect(remoteIp);
				Debug.Log("Socket connected to "+ serverSocket.RemoteEndPoint.ToString());
			}
			catch (ArgumentNullException ane){
				Debug.Log("ArgumentNullException : "+ ane.ToString()); 
			}
			catch(SocketException se){
				Debug.Log("SocketException : "+ se.ToString()); 
			}
			catch(Exception e){
				Debug.Log("Unexpected exception : "+ e.ToString()); 
			}
		}
		catch(Exception e){
			Debug.Log (e.ToString ());
		}
	}*/
	void OnApplicationQuit(){
		Send("-e-"+getLenStr(name)+name);
		client.Close();
		//serverSocket.Shutdown (SocketShutdown.Both);
		//serverSocket.Close ();
	}
	void SendGameState(){
		//+p00 posx, posy, posz "+p";
		string data = _player.transform.position.x.ToString() +","+_player.transform.position.y.ToString() +"," +_player.transform.position.z.ToString();
		data = "+p" + getLenStr (data) + data;
		Debug.Log (data);
		//Send (data);
	}
	void SpawnPlayer(){
		
	}
	// Update is called once per frame
	void Update () {
		_frameCount++;
		if (_frameCount >= _modCount) {
			_frameCount = 0;
			SendGameState ();
		}
		if (client.Available > 0 ) {
			IPEndPoint ipEnd = new IPEndPoint(IPAddress.Any, 0);
			Byte[] receiveBytes = client.Receive(ref ipEnd);
			string receivedData = Encoding.ASCII.GetString(receiveBytes);
			Debug.Log(receivedData);
			if (receivedData.Contains ("-n-")) {
				receivedData = receivedData.Remove (0, 5);
				GameObject.Find ("MainText").GetComponent<Text> ().text += "\n" + receivedData+" has Connected";
			}
			else if (receivedData.Contains ("-e-")) {
				receivedData = receivedData.Remove (0, 5);
				GameObject.Find ("MainText").GetComponent<Text> ().text += "\n" + receivedData+" has Disconnected";
			} 
			else {
				receivedData = receivedData.Remove (0, 5);
				GameObject.Find ("MainText").GetComponent<Text> ().text += "\n" + receivedData;
			}
		}
		if (Input.GetKeyDown (KeyCode.Space)) {
			Byte[] sendBytes = Encoding.ASCII.GetBytes("Is anybody there?");
			client.Send(sendBytes, sendBytes.Length);


			/*byte[] buffer = new byte[256];
			byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>"); 
			int byteSent = serverSocket.Send(msg);

			int bytesRec = serverSocket.Receive(buffer);
			Debug.Log("Echoed test = "+ Encoding.ASCII.GetString(buffer, 0, bytesRec));*/
		}
	}
	public void LogIn(){
		//get the text from input field
		string name;
		name = input.transform.FindChild("Text").GetComponent<Text>().text;
		//Debug.Log (name);
		if (name.Contains ("-n-"))
			return;
		_name = name;
		Send ("-n-"+getLenStr(name)+name);
		//on return of the message 
		Ask4Players();
	}
	void Ask4Players(){
		Send ("/g_a_players");
	}
	public void SendMyMessgae(){
		string text;
		text = _name+": " + GameObject.Find("ChatInput").transform.FindChild("InputField").transform.FindChild("Text").GetComponent<Text>().text;
		Send ("-m-"+getLenStr(text)+text);
		GameObject.Find("MainText").GetComponent<Text>().text += "\n"+text;

	}
	string getLenStr(string message){
		if (message.Length > 99)
			return "00";
		string len = "";
		if (message.Length < 10) {
			len ="0";
		}
		len += message.Length.ToString ();
		return len;
	}
	public void Send(string message){
		//Debug.Log (message);
		Byte[] sendBytes = Encoding.ASCII.GetBytes(message);
		client.Send(sendBytes, sendBytes.Length);
	}
}
