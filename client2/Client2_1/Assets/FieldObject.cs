using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FieldObject : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        start = DateTime.Now;
        now = DateTime.Now;
    }

    // Update is called once per frame
    void Update()
    {
        now = DateTime.Now;
        var interval = now - start;
        start = now;
        float dt = (float)interval.TotalMilliseconds;

        if (move)
        {
            Vector2 pre = new Vector2(transform.position.x,
                transform.position.z);
            pre = pre + (dt * spd * d);
            transform.position = new Vector3(pre.x,
                transform.position.y, pre.y);
        }
    }

    public float spd;
    public Vector2 d;
    public bool move;
    DateTime start;
    DateTime now;
}
