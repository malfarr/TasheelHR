<?php

function pie_backend_highcharts_line_chart($data) {
  $chart_data = array(
    'container_id' => $data['container_id'],
    'container_classes' => $data['container_class'],
    'chart_data' => array(
      'title' => hr_basic_fix_js_string_format($data['title']),
      'subtitle' => hr_basic_fix_js_string_format($data['subtitle']),      
      'y_title' => $data['y_title'],
      'x_categories' => $data['x_categories'],      
      'series_data' => array_values($data['series_data']),
    )
  );
  
  return theme('section_highcharts_line', $chart_data);    
}

function hr_backend_highcharts_pie_chart($data) {
  $chart_data = array(
    'container_id' => $data['container_id'],
    'container_classes' => $data['container_class'],
    'chart_data' => array(
      'title' => hr_basic_fix_js_string_format($data['title']),
      'subtitle' => hr_basic_fix_js_string_format($data['subtitle']),      
      'series_data' => array_values($data['series_data']),
    )
  );
  
  return theme('section_highcharts_pie', $chart_data);    
}

function hr_backend_highcharts_column_percentage($data) {
  $chart_data = array(
    'container_id' => $data['container_id'],
    'container_classes' => $data['container_class'],
    'chart_data' => array(
      'title' => hr_basic_fix_js_string_format($data['title']),
      'subtitle' => hr_basic_fix_js_string_format($data['subtitle']),      
      'series_data' => array_values($data['series_data']),
    )
  );
  if(isset($data['subtitle'])){
    $chart_data['chart_data']['subtitle'] = $data['subtitle'];
  }
  return theme('section_highcharts_column_percentage', $chart_data);    
}
