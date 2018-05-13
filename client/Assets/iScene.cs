using UnityEngine;
public class iScene {
	protected GameObject _sceneObj;
	public iScene(GameObject target){
		_sceneObj = target;
	}
	public virtual void Start(){}
	public virtual void Update () {
		
	}
	public virtual void ButtonHandle(ButtonEvents be){
	
	}
	public virtual void End(){
		
	}
}
