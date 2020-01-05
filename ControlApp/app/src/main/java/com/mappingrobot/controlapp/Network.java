package com.mappingrobot.controlapp;

import android.content.Context;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

public class Network {

    private static final String ESP_SERVER_URL = "http://192.168.0.20:80";
    private static final int TIMEOUT = 5000;
    private static RequestQueue queue;

    public static void init(Context ctx) {
        queue = Volley.newRequestQueue(ctx);
    }

    /** Requests a chunk of the intern map of the robot
     * The answer should contain the lines 3*i to 3*i+2 of the map */
    public static void requestMapChunk(final int chunkIndex) {

        String url = ESP_SERVER_URL + "/map_chunk?index=" + chunkIndex + "\n";

        System.out.println("Sent request " + url);
        // Request a string response from the provided URL.
        StringRequest stringRequest = new StringRequest(com.android.volley.Request.Method.GET, url,
                new com.android.volley.Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        RobotMap.updateMapChunk(chunkIndex, response);
                        System.out.println(response);
                    }
                }, new com.android.volley.Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        System.out.println(error.getMessage());
                    }
                }
        );
        stringRequest.setRetryPolicy(new DefaultRetryPolicy(
                TIMEOUT,
                DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT
        ));

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    /** Requests a change of the target position of the robot */
    public static void requestTargetChange(int x, int y) {

        String url = ESP_SERVER_URL + "/set_target?x=" + x + "&y=" + y + "\n";

        System.out.println("Sent request " + url);
        // Request a string response from the provided URL.
        StringRequest stringRequest = new StringRequest(com.android.volley.Request.Method.GET, url,
                new com.android.volley.Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        System.out.println(response);
                    }
                }, new com.android.volley.Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        System.out.println(error.getMessage());
                    }
                }
        );
        stringRequest.setRetryPolicy(new DefaultRetryPolicy(
                TIMEOUT,
                DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT
        ));

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    /** Requests the position and rotation of the robot */
    public static void requestPosition() {

        String url = ESP_SERVER_URL + "/position\n";

        System.out.println("Sent request " + url);
        // Request a string response from the provided URL.
        StringRequest stringRequest = new StringRequest(com.android.volley.Request.Method.GET, url,
                new com.android.volley.Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        System.out.println(response);
                        RobotTransform.update(response);
                    }
                }, new com.android.volley.Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        System.out.println(error.getMessage());
                    }
                }
        );
        stringRequest.setRetryPolicy(new DefaultRetryPolicy(
                TIMEOUT,
                DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT
        ));

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }
}
