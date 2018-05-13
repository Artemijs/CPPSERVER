using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LogIn : iBack {
	PlayerManager _playerCtrl;
	GameObject _loginError;
	public LogIn():base(){
		_playerCtrl = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ();
		_loginError = GameObject.Find ("E_LOGIN");
		_loginError.SetActive (false);
	}
	public override void Start (string message)
	{
		//00id
		Debug.Log("in log in "+message);
		int id = int.Parse(message.Substring(2, 2));
		if (id < 0) {
			//login alreayd existE_LOGIN
			_loginError.SetActive(true);
			return;
		}
		_playerCtrl.SetMyPlayerId (id);
		//next i must start loading all the shit 
		NetworkController nc = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		nc.Send (NetCalls.LOADPLAYERS, "", true, 10.0f);
	}
}
