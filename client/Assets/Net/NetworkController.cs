using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System;
using System.Text;

public class NetworkController : MonoBehaviour {
	Dictionary<int, iBack> _all_calls;	
	public string _ipAddressStr;
	public int _port;
	UdpClient _uClient;
	bool _wait4resp;
	float _time2wait;
	float _time;
	// Use this for initialization
	//i store the last sent message here in case it doesnt reach the destination adn needs to be resent
	NetMsg _cachedMsg;
	bool _ready = false;
	void Awake() {
		//_ipAddress = new IPAddress (IPAddress.Parse (ipAddressStr));
		_all_calls = new Dictionary<int, iBack> ();

		
	}
	public void Connect(string ip, int port){
		_port = port;
		_ipAddressStr = ip;
		try{
			_uClient = new UdpClient(port);
			//tcp on 5556 and udp on 5555
			_uClient.Connect(IPAddress.Parse(_ipAddressStr), 5556);
			_ready = true;
		}catch(Exception e){
			Debug.Log (e.ToString ());
		}
	}
	void Star(){
		_time2wait = 0;
		_wait4resp = false;
		_time = 0;
	}

	public void RegisterCallback(int key, iBack call){
		_all_calls.Add (key, call);
	}

	public int GetKeyFromStr(string s){
		//possible 00 - 99
		int ret ;
		int.TryParse(s.Substring(0, 2), out ret);
		return ret;
	}

	private void ParseMessage(string message){
		_all_calls [GetKeyFromStr (message)].Start (message);
	}

	void Update () {
		if (!_ready) return;
		if (_wait4resp) {
			_time += Time.deltaTime;
			if (_time >= _time2wait) {
				//resend the message
				Send(_cachedMsg);
				_time = 0;
			}
		}
		if (_uClient.Available > 0 ) {
			IPEndPoint ipEnd = new IPEndPoint(IPAddress.Any, 0);
			Byte[] receiveBytes = _uClient.Receive(ref ipEnd);
			string receivedData = Encoding.ASCII.GetString(receiveBytes);
			int id = GetStrId (receivedData);
			_all_calls [id].Start (receivedData);
			Debug.Log(id);

			if (_wait4resp) {
				_wait4resp = false;
				_time = 0;
			}
			
		}
	}

	public string GetTypeStr(int type){
		string str = "";
		if (type < 10) {
			//0x
			str += "0";
		}
		str += type.ToString ();
		return str;
	}

	string GetLenStr(string msg){
		if (msg.Length > 99)
			return "00";
		string len = "";
		if (msg.Length < 10) {
			len ="0";
		}
		Debug.Log ("THAT message len : " + msg.Length.ToString ());
		len += msg.Length.ToString ();
		return len;
	}

	public void Send(NetMsg msg){
		string message = GetTypeStr ((int)(msg.type)) + GetLenStr (msg.data) + msg.data;
		//Debug.Log (" Sending message : " + message);
		Byte[] sendBytes =  System.Text.Encoding.ASCII.GetBytes(message);
		_uClient.Send(sendBytes, sendBytes.Length);

		if (!msg.wait) return;
		_wait4resp = true;
		_time2wait = msg.time;
		_cachedMsg = msg;

	}

	public void Send(NetCalls calltype, string message, bool wait, float duration ){
		Debug.Log (" message len : " + message.Length.ToString ());
		Debug.Log (" Sending message : " + message);
		message = GetTypeStr ((int)(calltype)) + GetLenStr (message) + message;

		Byte[] sendBytes =  System.Text.Encoding.ASCII.GetBytes(message);
		_uClient.Send(sendBytes, sendBytes.Length);

		if (!wait) return;
		_wait4resp = wait;
		_time2wait = duration;
		_cachedMsg.type = calltype;
		_cachedMsg.data = message;
		_cachedMsg.wait = true;
		_cachedMsg.time = duration;

	}

	public void TCPSend(NetCalls calltype, string message){
		message = GetTypeStr ((int)(calltype)) + GetLenStr (message) + message;

		TcpClient client = new TcpClient(_ipAddressStr, _port);
		var msgBuff = System.Text.Encoding.ASCII.GetBytes(message);
		var stream = client.GetStream();
		stream.Write(msgBuff, 0, msgBuff.Length); //sends bytes to server

		//stream = new byte[128];
		byte[] response = new byte[128];
		int respLength = stream.Read(response, 0, response.Length); //gets next 128 bytes when sent to client
		ParseMessage(System.Text.Encoding.ASCII.GetString(response));
		//Console.Write(System.Text.Encoding.ASCII.GetString(message));
		stream.Close();
		client.Close();
	}

	public void End(){
		_uClient.Close ();
	}

	int GetStrId(string msg){
		return int.Parse (msg.Substring (0, 2));
	}
}

public static class StaticClass{
	public static string RemoveWhitespace(this string str) {
		return string.Join("", str.Split(default(string[]), StringSplitOptions.RemoveEmptyEntries));
	}
	public static float SEND_RATE = 10.0f/60.0f;

	public static string[] split(string s, char c){
		int len = s.Length;
		int i = 0;
		int nrOfc = 0;
		int first = -1;
		while (i < len) {
			//Debug.Log (s [i] + "  " + c + (s [i] == c).ToString ());
			if (s [i] ==  c) {
				nrOfc++;
				if (first == -1) {
					first = i;
				}
			}
			i++;
		}
		if (first == -1) {
			Debug.Log ("NOT FOUND ");
			return new string[0];
		}
		i = first;
		//+1 because i vvant everything before a c and after c so there maybe only 1 c but 2 items in array
		string[] retVal = new string[nrOfc+1];
		int nr = 0;
		Debug.Log ("i : " + i.ToString ());
		retVal [nr] = s.Substring (0, i);
		if (nrOfc == 1)
			return retVal;
		nr++;
		i++;
		first++;
		while (i < len) {
			if (s [i] == c) {
				retVal [nr] = s.Substring (first, (i-first));
				first = i;
				first++;
				nr++;
			}
			i++;
		}
		retVal [nr] = s.Substring (first, (i-first));
		return retVal;

	}
}