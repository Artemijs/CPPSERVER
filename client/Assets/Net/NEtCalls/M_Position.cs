using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class M_Position: iBack {
	PlayerManager _pMgr;
	public M_Position():base(){
		_pMgr = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ();
	}
	public override void Start (string message)
	{
		message = message.Substring (4, message.Length - 4);
		string[] allData = message.Split ('#');
		int pId = int.Parse (allData [0]);

		string ss = allData [1];

		string[] allSs = ss.Split(',');
		Vector3 pos = new Vector3 (float.Parse (allSs [0]),float.Parse (allSs [1]), float.Parse (allSs [2]));
		string[] allrot = allData [2].Split (',');
		Vector3 rot = new Vector3 (float.Parse (allrot [0]),float.Parse (allrot [1]), float.Parse (allrot [2]));
		_pMgr.UpdatePlayerBasic (pId, pos, rot);
	}
}
