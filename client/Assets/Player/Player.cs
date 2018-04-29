using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {
	PlayerStats _stats;
	// Use this for initialization
	float _speed;
	Vector3 _dir;
	Vector3 _target;
	float _time = 0.0f;
	bool _move;
	GameObject _temp;
	void Start () {
		_move = false;
		_temp = GameObject.Find("Temp2");
	}
	
	// Update is called once per frame
	void Update () {
		if (!_move)
			return;
		//if (Vector3.Distance (transform.position, _target) < 0.1f)
		//	_move = false;
		_time+=Time.deltaTime;
		if (Vector3.Dot(_dir, (_target - transform.position).normalized) < 0) {
			_move = false;
			_time = 0;
			transform.position = _target;
			return;
		}
		Vector3 pos = transform.position;
		//Debug.Log ("________");
		//Debug.Log ( _speed);
		//Debug.Log (_dir * _speed);
		//Debug.Log (Vector3.Distance (transform.position, _target));
		//Debug.Log (Vector3.Dot(_dir, (_target - transform.position).normalized));
		//Debug.Log ("________");
		pos += (_dir * _speed);
		transform.position = pos;
	}
	public void Move(Vector3 target){

		_move = true;

		float dist = Vector3.Distance (transform.position, target);
		_dir = (target - transform.position).normalized;
		//Debug.Log (dist);Debug.Log (StaticClass.SEND_RATE);
		_speed = dist / (StaticClass.SEND_RATE * 60); // StaticClass.SEND_RATE/dist;
		_target = target;
		Debug.Log ("POS " + transform.position.ToString () + " TARGET " + target.ToString () + " DIR " + _dir);
		//_temp.transform.position = target;
	}
	public void SetStats(PlayerStats ps){
		_stats = ps;
	}
	public PlayerStats GetStats(){
		return _stats;
	}
}
