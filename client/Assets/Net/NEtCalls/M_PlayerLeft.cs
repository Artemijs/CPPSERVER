using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class M_PlayerLeft :iBack {
	PlayerManager _playerMngr;
	public M_PlayerLeft () : base (){
		_playerMngr = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ();
	}

	public override void Start (string message)
	{
		int id = int.Parse (message.Substring (2, 2));
		Debug.Log ("PLAYER LEFT " + message + " " + id.ToString ());
		_playerMngr.RemovePlayer (id);
	}
}
