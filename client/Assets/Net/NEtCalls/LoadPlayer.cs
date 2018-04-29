using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoadPlayer : iBack {
	PlayerManager pc;
	//checks if start has ben already called once
	bool _poorDecisions;
	public LoadPlayer():base(){
		_poorDecisions = false;
		pc = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ();
	}
	// Use this for initialization
	public override void Start (string msg) {
		Debug.Log ("in start LOAD PLAYERS:" + msg);
		msg = msg.Substring (2, msg.Length-2);
		Debug.Log ("ATTEMPTING SHENANIGHANS");
		/*for (int i = 0; i < msg.Length; i++) {
			ssssss += msg [i];
			ssssss += '-';
		}
		Debug.Log (ssssss);*/
		//string[] allPs = StaticClass.split (msg, '|');  //msg.Split ('-');
		string[] allPs = msg.Split ('|');
		//minus 1 because the split includes the after of the last | vvhich is nothing
		for (int i = 0; i < allPs.Length-1; i++) {
			Debug.Log (" len " + allPs [i].Length.ToString ());
			if (allPs [i].Length  <4)
				continue;
			//Debug.Log (allPs [i] + " "+allPs[i].Length.ToString());
			//string[] allVs = StaticClass.split (allPs[i], '#');
			string[] allVs = allPs[i].Split('#');
			Debug.Log(" ALL ALUEs ");
			Debug.Log (allVs [0]);Debug.Log (allVs [1]);Debug.Log (allVs [2]);
			PlayerStats ps = GetStats (allVs [0]);
			int id = pc.CreatePlayer (ps);
			pc.SetPlayerBasic (id, getVector (allVs [1]), getVector (allVs [2]));
		}
		pc.SortPlayerArr ();
		if (_poorDecisions)
			return;
		pc.gameObject.GetComponent<Main> ().ChangeScene (SceneId.GAME);
		_poorDecisions = true;
	}
	Vector3 getVector(string s){
		string[] arr = s.Split(',');
		Debug.Log (s);
		Vector3 pos = new Vector3 (float.Parse (arr [0]),float.Parse (arr [1]), float.Parse (arr [2]));
		/*nameId = allVs[2].Split(',');
		Vector3 rot = new Vector3 (float.Parse (nameId [0]),float.Parse (nameId [1]), float.Parse (nameId [2]));*/
		return pos;
	}
	PlayerStats GetStats(string s){
		PlayerStats ps;
		string[] allVs = s.Split (',');
		/*for (int i = 0; i < allVs.Length; i++) {
			Debug.Log (allVs [i]);
		}*/
		//name,id
		//Debug.Log(allVs.Length);

		ps.name = allVs[0];
		ps.id = int.Parse (allVs [1]);


		return ps;
	}
	

}
