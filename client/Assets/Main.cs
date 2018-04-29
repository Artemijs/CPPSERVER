using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
public class Main : MonoBehaviour {
	List<iScene> _all_scenes;
	int _current_scene;
	// Use this for initialization
	public GameObject[] _scenes;
	void Start () {
		_current_scene = (int)(SceneId.LOGIN);
		_all_scenes = new List<iScene> ();
		_all_scenes.Add (new S_LoginScene (_scenes[0]));
		_all_scenes.Add (new S_Game (_scenes[1]));
		_all_scenes [_current_scene].Start ();
	}
	
	// Update is called once per frame
	void Update () {
		_all_scenes [_current_scene].Update ();
	}
	public void ButtonHandle(int be){
		_all_scenes [_current_scene].ButtonHandle ((ButtonEvents)(be));
	}
	void OnApplicationQuit(){
		NetworkController nc = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		PlayerManager pc = GameObject.Find("GameMaster").GetComponent<PlayerManager>();

		nc.Send (NetCalls.END, nc.GetTypeStr(pc.GetMyPlayer().GetStats().id), false, 0);
		nc.End ();
	}
	public void ChangeScene(SceneId next){
		_all_scenes [_current_scene].End ();
		_current_scene = (int)(next);
		_all_scenes [_current_scene].Start ();
	}

}
