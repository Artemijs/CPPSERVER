using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {
	public bool _3rdPerson;
	Rigidbody _body;
	public Transform _camera;
	float _distance = 100.0f;
	float _currentX = 0.0f;
	float _currentY = 0.0f;
	int _speed;
	float _sendRate;
	float _time;
	bool _moved;
	// Use this for initialization
	NetworkController _net;
	Player _myPlayer;
	void Start () {
		_speed = 2000;
		_moved = false;
		_3rdPerson = true;
		//10 times a second
		_sendRate = StaticClass.SEND_RATE;
		_time = 0;
		_body = gameObject.GetComponent<Rigidbody> ();
		_net = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		_myPlayer = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ().GetMyPlayer ();
	}

	void sendData(){
		if (!_moved) return;
		_moved = false;
		string pos = transform.position.ToString ();
		pos = pos.Substring (1, pos.Length - 2);
		string rot = transform.rotation.eulerAngles.ToString ();
		pos = _net.GetTypeStr(_myPlayer.GetStats().id) + "#" + StaticClass.RemoveWhitespace (pos) + "#" + 
			StaticClass.RemoveWhitespace (rot.Substring(1, rot.Length-2));
		Debug.Log (pos);
		_net.Send (NetCalls.POSITION, pos, false, 0);
	}

	void Update () {
		_time += Time.deltaTime;
		if (_time >= _sendRate) {
			sendData ();
			_time = 0;
		}
		_currentX += Input.GetAxis("Mouse X");
		_currentY += -Input.GetAxis("Mouse Y");
		if ( Input.GetAxis("Mouse ScrollWheel") != 0) {
			_3rdPerson = !_3rdPerson;
		}
		float xDir = Input.GetAxis("Horizontal");
		float zDir = Input.GetAxis("Vertical");
		//get my dir 
		Vector3 fDir = _camera.forward;
		fDir.y = 0;
		Vector3 sDir = Vector3.Cross (fDir, new Vector3(0, 1, 0));

		Vector3 vel = (fDir * zDir) + (sDir * -xDir);// new Vector3 (xDir * speed, 0, zDir * speed);
		if(vel.magnitude != 0){
			_moved = true;
		}
		_body.AddForce( vel* _speed );
	}
	void LateUpdate(){
		if (_3rdPerson) {
			Vector3 dir = new Vector3 (0, 0, -_distance);
			Quaternion rotation = Quaternion.Euler (_currentY, _currentX, 0);
			_camera.position = transform.position + rotation * dir;
			_camera.LookAt (transform.position);
			Vector3 forvvard = Vector3.Cross (Vector3.up, _camera.right);
			transform.LookAt (transform.position + forvvard);
		} else {
			_camera.position = transform.position;
			//this to be camera child
			_camera.GetChild(0).localRotation = Quaternion.AngleAxis (	_currentY, Vector3.right);
			//this to be camera parent
			_camera.localRotation = Quaternion.AngleAxis (_currentX, transform.up);
		}
	}
}
